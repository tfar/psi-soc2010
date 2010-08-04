#ifndef SERVERROOMLISTMODEL_H
#define SERVERROOMLISTMODEL_H

#include <QAbstractItemModel>

#include "psicon.h"
#include "psiaccount.h"

class ServerRoomListModel : public QAbstractItemModel
{
    Q_OBJECT
public:
	explicit ServerRoomListModel(QObject *parent = 0);
	ServerRoomListModel(PsiCon *con, PsiAccount *acc, QString domain);

	void setShowNumberOfOccupants(bool show);
signals:

public slots:

private:
	PsiCon *controller_;
	PsiAccount *account_;
	bool showOcc_;

	int columnCount(const QModelIndex &parent) const;
	int rowCount(const QModelIndex & parent) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	QVariant data(const QModelIndex &index, int role) const;
	QModelIndex index(int row, int column, const QModelIndex &parent) const;
	QModelIndex parent(const QModelIndex &index) const;

};

#endif // SERVERROOMLISTMODEL_H
