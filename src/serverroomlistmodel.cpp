#include "serverroomlistmodel.h"

int ServerRoomListModel::columnCount(const QModelIndex &parent) const {
	return 2 + (showOcc_ ? 1 : 0);
}

int ServerRoomListModel::rowCount(const QModelIndex & parent) const {
	// return number of rooms
	return 1;
}

QVariant ServerRoomListModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (role != Qt::DisplayRole) return QVariant();
	if (section == 0) return QVariant("Name");
	if (section == 1) return QVariant("Room JID");
	if (section == 2) return QVariant("# of Occupants");
	return QVariant();
}

QVariant ServerRoomListModel::data(const QModelIndex &index, int role) const {
	return QVariant("Hello");
}

QModelIndex ServerRoomListModel::index(int row, int column, const QModelIndex &parent) const {
	return createIndex(row, column, 0);
}

QModelIndex ServerRoomListModel::parent(const QModelIndex &index) const {
	return QModelIndex();
}

ServerRoomListModel::ServerRoomListModel(QObject *parent) :
    QAbstractItemModel(parent)
{
	showOcc_ = false;
}

ServerRoomListModel::ServerRoomListModel(PsiCon *con, PsiAccount *acc, QString domain) :
	QAbstractItemModel(0)
{
	controller_ = con;
	account_ = acc;
	showOcc_ = false;
}

void ServerRoomListModel::setShowNumberOfOccupants(bool show) {
	showOcc_ = show;
	emit headerDataChanged(Qt::Horizontal, 0, 1 + (showOcc_ ? 1 : 0));
}
