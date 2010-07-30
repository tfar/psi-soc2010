#include "recentandbookmarkedroomsmodel.h"

int RecentAndBookmarkedRoomsModel::columnCount(const QModelIndex &parent) const {
	fprintf(stderr, "RecentAndBookmarkedRoomsModel::columnCount()\n");
	return 2;
}

int RecentAndBookmarkedRoomsModel::rowCount(const QModelIndex & parent) const {
	fprintf(stderr, "RecentAndBookmarkedRoomsModel::rowCount()\n");
	// return number of bookmark items
	int count = recentRooms_.length();
	count += bookmarks_.length();
	return count;
}

QVariant RecentAndBookmarkedRoomsModel::headerData(int section, Qt::Orientation orientation, int role) const {
	fprintf(stderr, "RecentAndBookmarkedRoomsModel::headerData()\n");
	if (role != Qt::DisplayRole) return QVariant();
	if (section == 0) return QVariant("Name");
	if (section == 1) return QVariant("Room JID");
	return QVariant();
}

QVariant RecentAndBookmarkedRoomsModel::data(const QModelIndex &index, int role) const {
	fprintf(stderr, "RecentAndBookmarkedRoomsModel::data()\n");
	if (role != Qt::DisplayRole) return QVariant();
	if (index.row() < recentRooms_.length()) {
		if (index.column() == 0) return QVariant(QString("%1. Last Entered Room").arg(1 + index.row()));
		if (index.column() == 1) return QVariant(recentRooms_.at(index.row()));
	} else {
		int row = index.row() - recentRooms_.length();
		if (index.column() == 0) return QVariant(bookmarks_.at(row).name());
		if (index.column() == 1) return QVariant(bookmarks_.at(row).jid().full());
	}
}

QModelIndex RecentAndBookmarkedRoomsModel::index(int row, int column, const QModelIndex &parent) const {
	fprintf(stderr, "RecentAndBookmarkedRoomsModel::index()\n");
	return createIndex(row, column, 0);
}

QModelIndex RecentAndBookmarkedRoomsModel::parent(const QModelIndex &index) const {
	fprintf(stderr, "RecentAndBookmarkedRoomsModel::parent()\n");
	return QModelIndex();
}

RecentAndBookmarkedRoomsModel::RecentAndBookmarkedRoomsModel(QObject *parent) :
    QAbstractItemModel(parent)
{

}
RecentAndBookmarkedRoomsModel::RecentAndBookmarkedRoomsModel(PsiCon *con, PsiAccount *acc) :
	QAbstractItemModel(0)
{
	controller_ = con;
	account_ = acc;

	recentRooms_ = controller_->recentGCList();
	bookmarks_ = account_->bookmarkManager()->conferences();
}
