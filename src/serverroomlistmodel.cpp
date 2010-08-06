#include "serverroomlistmodel.h"

#include <QtAlgorithms>

#define MAX_CONCURRENT_REQUESTS 10

int ServerRoomListModel::columnCount(const QModelIndex &parent) const {
	Q_UNUSED(parent);
	return 2 + (showOcc_ ? 1 : 0);
}

int ServerRoomListModel::rowCount(const QModelIndex & parent) const {
	Q_UNUSED(parent);
	// return number of rooms
	return roomList_.length();
}

QVariant ServerRoomListModel::headerData(int section, Qt::Orientation orientation, int role) const {
	Q_UNUSED(orientation);
	if (role != Qt::DisplayRole) return QVariant();
	if (section == 0) return QVariant("Name");
	if (section == 1) return QVariant("Room JID");
	if (section == 2) return QVariant("# of Occupants");
	return QVariant();
}

QVariant ServerRoomListModel::data(const QModelIndex &index, int role) const {
	if (role != Qt::DisplayRole) return QVariant();
	//fprintf(stderr, "\tdata for ( %d | %d )\n", index.column(), index.row());
	MUCUtility::MUCRoom room = roomList_.at(index.row());
	if (index.column() == 0) return QVariant(room.name);
	if (index.column() == 1) return QVariant(room.jid.full());
	if (index.column() == 2) {
		if (room.occupants == -1) {
			ServerRoomListModel *nonconst_this = const_cast<ServerRoomListModel*>(this);
			nonconst_this->fetchNoOfOccupants(index.row());
			return QVariant("Loading...");
		} else {
			return QVariant(room.occupants);
		}
	}
	return QVariant();
}

QModelIndex ServerRoomListModel::index(int row, int column, const QModelIndex &parent) const {
	Q_UNUSED(parent);
	return createIndex(row, column, 0);
}

QModelIndex ServerRoomListModel::parent(const QModelIndex &index) const {
	Q_UNUSED(index);
	return QModelIndex();
}

bool mucRoomNameLessThan(const MUCUtility::MUCRoom &r1, const MUCUtility::MUCRoom &r2) {
	return r1.name.toLower() < r2.name.toLower();
}

bool mucRoomJidLessThan(const MUCUtility::MUCRoom &r1, const MUCUtility::MUCRoom &r2) {
	return r1.jid.full() < r2.jid.full();
}

bool mucRoomOccupantsLessThan(const MUCUtility::MUCRoom &r1, const MUCUtility::MUCRoom &r2) {
	return r1.occupants < r2.occupants;
}

bool mucRoomNameGreaterThan(const MUCUtility::MUCRoom &r1, const MUCUtility::MUCRoom &r2) {
	return r1.name.toLower() > r2.name.toLower();
}

bool mucRoomJidGreaterThan(const MUCUtility::MUCRoom &r1, const MUCUtility::MUCRoom &r2) {
	return r1.jid.full() > r2.jid.full();
}

bool mucRoomOccupantsGreaterThan(const MUCUtility::MUCRoom &r1, const MUCUtility::MUCRoom &r2) {
	return r1.occupants > r2.occupants;
}



void ServerRoomListModel::sort(int column, Qt::SortOrder order) {
	//fprintf(stderr, "\t column: %d, order: %d\n", column, order);
	if (column == 0) qSort(roomList_.begin(), roomList_.end(), !order ? mucRoomNameLessThan : mucRoomNameGreaterThan);
	if (column == 1) qSort(roomList_.begin(), roomList_.end(), !order ? mucRoomJidLessThan : mucRoomJidGreaterThan);
	if (column == 2) qSort(roomList_.begin(), roomList_.end(), !order ? mucRoomOccupantsLessThan : mucRoomOccupantsGreaterThan);

	// get jidToRow_ in sync
	for (int n = 0; n < roomList_.length(); ++n) {
		MUCUtility::MUCRoom room = roomList_.at(n);
		if (jidToRow_.contains(room.jid.full())) jidToRow_.insert(room.jid.full(), n);
	}
	emit layoutChanged();
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
	mucutility_ = NULL;
	progressBar_ = NULL;
	showOcc_ = false;
	concurrentRequests_ = 0;
	setRoom(Jid(domain));
}

void ServerRoomListModel::setProgressBar(QProgressBar *pb) {
	progressBar_ = pb;
	mucutility_->setProgressWidget(progressBar_);
}

void ServerRoomListModel::setRoom(Jid roomjid) {
	roomjid_ = roomjid;
	if (mucutility_) delete mucutility_;
	mucutility_ = new MUCUtility(account_);
	if (progressBar_) mucutility_->setProgressWidget(progressBar_);
	connect(mucutility_, SIGNAL(receivedListOfRooms(QList<MUCUtility::MUCRoom>)), SLOT(receivedListOfRooms(QList<MUCUtility::MUCRoom>)));
	connect(mucutility_, SIGNAL(receivedNoOfOccupants(Jid,ulong)), SLOT(receivedNoOfOccupants(Jid,ulong)));
	mucutility_->determineListOfRooms(roomjid_);
	rowsWithOccupantsInfoFinished_ = rowsWithOccupantsInfoWanted_ = 1;
	jidToRow_.clear();
}

void ServerRoomListModel::setShowNumberOfOccupants(bool show) {
	showOcc_ = show;
	emit headerDataChanged(Qt::Horizontal, 0, 1 + (showOcc_ ? 1 : 0));
}

// MUCUtility stuff
void ServerRoomListModel::receivedListOfRooms(QList<MUCUtility::MUCRoom> roomList) {
	emit beginResetModel();
	roomList_ = roomList;
	emit endResetModel();
}

void ServerRoomListModel::updateProgress() {
	if(progressBar_) {
		double percentage = (double)rowsWithOccupantsInfoFinished_ / (double)rowsWithOccupantsInfoWanted_ * 100.0;
		progressBar_->setValue(percentage);
	}
}

void ServerRoomListModel::fetchNoOfOccupants(unsigned long row) {
	QString jid = roomList_.at(row).jid.full();
	if (jidToRow_.contains(jid)) return;
	++rowsWithOccupantsInfoWanted_;
	jidToRow_.insert(jid, row);
	//fprintf(stderr, "\tfetch # of occupants for %s\n", roomList_.at(row).jid.full().toUtf8().data());
	if (concurrentRequests_ <= MAX_CONCURRENT_REQUESTS) {
		++concurrentRequests_;
		mucutility_->determineNoOfOccupants(Jid(roomList_.at(row).jid));
	} else {
		occupantsRequestList_.append(row);
	}
	updateProgress();
}

void ServerRoomListModel::receivedNoOfOccupants(Jid roomjid, unsigned long occupants) {
	--concurrentRequests_;
	int result_for_row = jidToRow_.value(roomjid.full(), -1);
	Q_ASSERT(result_for_row != -1);
	//jidToRow_.remove(roomjid.full());
	MUCUtility::MUCRoom room = roomList_.value(result_for_row);
	room.occupants = occupants;
	roomList_[result_for_row] = room;
	//fprintf(stderr, "\tEmit row changed: %d\n", result_for_row);
	// TODO: less emits
	emit dataChanged(createIndex(result_for_row, 2), createIndex(result_for_row, 2));
	if (!occupantsRequestList_.empty()) {
		++concurrentRequests_;
		mucutility_->determineNoOfOccupants(Jid(roomList_.at(occupantsRequestList_.takeFirst()).jid));
	}
	++rowsWithOccupantsInfoFinished_;
	updateProgress();
}
