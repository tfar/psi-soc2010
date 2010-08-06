#ifndef RECENTANDBOOKMARKEDROOMSMODEL_H
#define RECENTANDBOOKMARKEDROOMSMODEL_H

#include <QAbstractItemModel>
#include <QList>

#include "psicon.h"
#include "psiaccount.h"
#include "bookmarkmanager.h"
#include "xmpp_jid.h"

class RecentAndBookmarkedRoomsModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit RecentAndBookmarkedRoomsModel(QObject *parent = 0);
	RecentAndBookmarkedRoomsModel(PsiCon *con, PsiAccount *acc);

	QList<Jid> getJidListForModelIndexList(QModelIndexList selectedIndices) const;
signals:

public slots:


private:
	PsiCon *controller_;
	PsiAccount *account_;

	QList<ConferenceBookmark> bookmarks_;
	QList<QString> recentRooms_;

	int columnCount(const QModelIndex &parent) const;
	int rowCount(const QModelIndex & parent) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	QVariant data(const QModelIndex &index, int role) const;
	QModelIndex index(int row, int column, const QModelIndex &parent) const;
	QModelIndex parent(const QModelIndex &index) const;

};

#endif // RECENTANDBOOKMARKEDROOMSMODEL_H
