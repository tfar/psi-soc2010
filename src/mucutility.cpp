#include "mucutility.h"

#include "xmpp_tasks.h"

void MUCUtility::determine_muc_disco_items_finished() {
	JT_DiscoItems *jt = qobject_cast<JT_DiscoItems*>(sender());
	Q_ASSERT(jt != NULL);
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
		QString found_service = "";
		bool is_server = false;
		foreach (DiscoItem::Identity ident, identities) {
			if (ident.category == "conference" &&
				jt->item().features().canGroupchat()) found_service = jt->jid().full();
			if (ident.category == "server") is_server = true;
		}

		if (found_service == "") {
			if (is_server) {
				// query items
				JT_DiscoItems *jt_items = new JT_DiscoItems(account_->client()->rootTask());
				connect(jt_items, SIGNAL(finished()), SLOT(determine_muc_disco_items_finished()));
				jt_items->get(jt->jid());
				jt_items->go(true);
			} else {
				if (determine_muc_task_queue_.empty()) {
					// emit signal with empty string indicating MUC service couldn't be found
					emit receivedMUCService(QString());
				} else {
					determine_muc_task_queue_.takeFirst()->go(true);
				}
			}
		} else {
			// emit signal
			foreach(Task *t, determine_muc_task_queue_) delete t;
			determine_muc_task_queue_.clear();
			emit receivedMUCService(found_service);
		}
	}
}

MUCUtility::MUCUtility(QObject *parent) :
    QObject(parent)
{

}

MUCUtility::MUCUtility(PsiAccount *acc) : QObject(0) {
	account_ = acc;
}

void MUCUtility::determineMUCServiceForDomain(const Jid &domain) {
	JT_DiscoInfo *jt = new JT_DiscoInfo(account_->client()->rootTask());
	connect(jt, SIGNAL(finished()), SLOT(determine_muc_disco_finished()));
	jt->get(domain.domain());
	jt->go(true);
}
