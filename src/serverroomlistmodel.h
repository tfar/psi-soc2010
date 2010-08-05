#ifndef SERVERROOMLISTMODEL_H
#define SERVERROOMLISTMODEL_H

#include <QAbstractItemModel>
#include <QProgressBar>

#include "psicon.h"
#include "psiaccount.h"
#include "mucutility.h"
#include "jidutil.h"

class ServerRoomListModel : public QAbstractItemModel
{
    Q_OBJECT
public:
	explicit ServerRoomListModel(QObject *parent = 0);
	ServerRoomListModel(PsiCon *con, PsiAccount *acc, QString domain);

	void setProgressBar(QProgressBar *pb);
	void setRoom(Jid roomjid);
	void setShowNumberOfOccupants(bool show);
signals:

public slots:

private slots:
	void receivedListOfRooms(QList<MUCUtility::MUCRoom> roomList);

private:
	PsiCon *controller_;
	PsiAccount *account_;
	bool showOcc_;
	MUCUtility *mucutility_;
	QList<MUCUtility::MUCRoom> roomList_;
	Jid roomjid_;
	QProgressBar *progressBar_;

	int columnCount(const QModelIndex &parent) const;
	int rowCount(const QModelIndex & parent) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	QVariant data(const QModelIndex &index, int role) const;
	QModelIndex index(int row, int column, const QModelIndex &parent) const;
	QModelIndex parent(const QModelIndex &index) const;

};

#endif // SERVERROOMLISTMODEL_H
