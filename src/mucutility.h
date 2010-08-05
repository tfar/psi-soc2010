#ifndef MUCUTILITY_H
#define MUCUTILITY_H

#include <QObject>
#include <QProgressBar>

#include "psiaccount.h"
#include "xmpp_jid.h"
#include "xmpp_task.h"

class MUCUtility : public QObject
{
    Q_OBJECT
public:
	struct MUCRoom {
		QString name;
		QString jid;
		int occupants;

		MUCRoom() { occupants = -1; }
	};

	explicit MUCUtility(QObject *parent = 0);
	MUCUtility(PsiAccount *acc);

	void setProgressWidget(QProgressBar *bar);

	void determineMUCServiceForDomain(const Jid &domain);
	void determineListOfRooms(const Jid &domain);
	void determineNoOfOccupants(const Jid &roomjid);
signals:
	void receivedMUCService(Jid host);
	void receivedListOfRooms(QList<MUCUtility::MUCRoom> roomList);
	void receivedNoOfOccupants(Jid roomjid, unsigned long number);

private slots:
	void determine_muc_disco_items_finished();
	void determine_muc_disco_finished();
	void determine_roomlist_disco_items_finished();
	void determine_no_of_occupants_disco_items_finished();

public slots:

private:
	PsiAccount *account_;
	QProgressBar *bar_;
	QList<Task*> determine_muc_task_queue_;

	void setProgress(double percent);
};

#endif // MUCUTILITY_H
