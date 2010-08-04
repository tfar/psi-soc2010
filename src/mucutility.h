#ifndef MUCUTILITY_H
#define MUCUTILITY_H

#include <QObject>

class MUCUtility : public QObject
{
    Q_OBJECT
public:
	struct MUCRoom {
		QString name;
		QString jid;
	};

	explicit MUCUtility(QObject *parent = 0);

signals:

public slots:
	void receivedMUCService(QString host);
	void receivedListOfRooms(QString host, QList<MUCRoom> roomList);
	void receivedNoOfOccupants(QString room, unsigned long no);
};

#endif // MUCUTILITY_H
