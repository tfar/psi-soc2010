#include "mucutility.h"

#include "xmpp_tasks.h"

void MUCUtility::determine_muc_disco_items_finished() {
	JT_DiscoItems *jt = qobject_cast<JT_DiscoItems*>(sender());
	Q_ASSERT(jt != NULL);
	setProgress(66);
	foreach(DiscoItem item, jt->items()) {
		JT_DiscoInfo *jt_new = new JT_DiscoInfo(account_->client()->rootTask());
		if (item.jid().full().contains(QRegExp("conference|room|muc|chat")))
			determine_muc_task_queue_.push_front(jt_new);
		else
			determine_muc_task_queue_.push_back(jt_new);
		connect(jt_new, SIGNAL(finished()), SLOT(determine_muc_disco_finished()));
		jt_new->get(item.jid());
	}
	if (!determine_muc_task_queue_.empty())
		determine_muc_task_queue_.takeFirst()->go(true);
	else
		emit receivedMUCService(QString());
}

void MUCUtility::determine_muc_disco_finished() {
	JT_DiscoInfo *jt = qobject_cast<JT_DiscoInfo*>(sender());
	Q_ASSERT(jt != NULL);
	if (jt) {
		const QList<DiscoItem::Identity> identities = jt->item().identities();
		Jid found_service = "";
		bool is_server = false;
		foreach (DiscoItem::Identity ident, identities) {
			if (ident.category == "conference" &&
				jt->item().features().canGroupchat()) found_service = jt->jid();
			if (ident.category == "server") is_server = true;
		}

		if (found_service == "") {
			if (is_server) {
				// query items
				JT_DiscoItems *jt_items = new JT_DiscoItems(account_->client()->rootTask());
				connect(jt_items, SIGNAL(finished()), SLOT(determine_muc_disco_items_finished()));
				jt_items->get(jt->jid());
				jt_items->go(true);
				setProgress(33);
			} else {
				if (determine_muc_task_queue_.empty()) {
					// emit signal with empty string indicating MUC service couldn't be found
					setProgress(100);
					emit receivedMUCService(QString());
				} else {
					determine_muc_task_queue_.takeFirst()->go(true);
				}
			}
		} else {
			// emit signal
			foreach(Task *t, determine_muc_task_queue_) delete t;
			determine_muc_task_queue_.clear();
			setProgress(100);
			emit receivedMUCService(found_service);
		}
	}
}

void MUCUtility::determine_roomlist_disco_items_finished() {
	JT_DiscoItems *jt = qobject_cast<JT_DiscoItems*>(sender());
	Q_ASSERT(jt != NULL);
	QList<MUCRoom> roomlist;
	foreach(DiscoItem item, jt->items()) {
		MUCRoom room;
		room.jid = item.jid().full();
		room.name = item.name();
		roomlist.append(room);
	}
	setProgress(100);
	emit receivedListOfRooms(roomlist);
}

void MUCUtility::determine_no_of_occupants_disco_items_finished() {
	JT_DiscoItems *jt = qobject_cast<JT_DiscoItems*>(sender());
	Q_ASSERT(jt != NULL);
	if (!jt->success()) receivedNoOfOccupants(jt->jid(), 0);
	emit receivedNoOfOccupants(jt->jid(), jt->items().length());
}

void MUCUtility::setProgress(double percent) {
	if (bar_) {
		bar_->setValue((int)percent);
	}
}

MUCUtility::MUCUtility(QObject *parent) :
    QObject(parent)
{

}

MUCUtility::MUCUtility(PsiAccount *acc) : QObject(0) {
	account_ = acc;
	bar_ = NULL;
}

void MUCUtility::setProgressWidget(QProgressBar *bar) {
	bar_ = bar;
	bar_->setMinimum(0);
	bar_->setMaximum(100);
}

void MUCUtility::determineMUCServiceForDomain(const Jid &domain) {
	JT_DiscoInfo *jt = new JT_DiscoInfo(account_->client()->rootTask());
	connect(jt, SIGNAL(finished()), SLOT(determine_muc_disco_finished()));
	jt->get(domain.domain());
	jt->go(true);
	setProgress(0);
}

void MUCUtility::determineListOfRooms(const Jid &domain) {
	JT_DiscoItems *jt_items = new JT_DiscoItems(account_->client()->rootTask());
	connect(jt_items, SIGNAL(finished()), SLOT(determine_roomlist_disco_items_finished()));
	jt_items->get(domain);
	jt_items->go(true);
	setProgress(50);
}

void MUCUtility::determineNoOfOccupants(const Jid &roomjid) {
	JT_DiscoItems *jt_items = new JT_DiscoItems(account_->client()->rootTask());
	connect(jt_items, SIGNAL(finished()), SLOT(determine_no_of_occupants_disco_items_finished()));
	jt_items->get(roomjid);
	jt_items->go(true);
}
