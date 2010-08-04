#ifndef MUCUTILITY_H
#define MUCUTILITY_H

#include <QObject>

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
	};

	explicit MUCUtility(QObject *parent = 0);
	MUCUtility(PsiAccount *acc);

	void determineMUCServiceForDomain(const Jid &domain);
signals:
	void receivedMUCService(QString host);
	void receivedListOfRooms(QString host, QList<MUCRoom> roomList);
	void receivedNoOfOccupants(QString room, unsigned long no);

private slots:
	void determine_muc_disco_items_finished();
	void determine_muc_disco_finished();

public slots:

private:
	PsiAccount *account_;
	QList<Task*> determine_muc_task_queue_;
};

#endif // MUCUTILITY_H
