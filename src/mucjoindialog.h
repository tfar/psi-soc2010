#ifndef MUCJOINDIALOG_H
#define MUCJOINDIALOG_H

#include <QDialog>
#include <QAbstractItemModel>

#include "PsiCon.h"

namespace Ui {
    class MUCJoinDialog;
}

class MUCBookmarkModel : public QAbstractItemModel
{
	Q_OBJECT
public:
	MUCBookmarkModel(QWidget *parent = 0);
	~MUCBookmarkModel();

	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &index) const ;
};

class MUCJoinDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MUCJoinDialog(QWidget *parent = 0);
	MUCJoinDialog(PsiCon *, PsiAccount *);
    ~MUCJoinDialog();

private slots:
	void updateIdentity(PsiAccount*);

private:
    Ui::MUCJoinDialog *ui;

	PsiCon *controller_;
	PsiAccount *account_;

	void initializeUI();
};

#endif // MUCJOINDIALOG_H
