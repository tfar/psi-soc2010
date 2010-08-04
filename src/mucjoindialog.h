#ifndef MUCJOINDIALOG_H
#define MUCJOINDIALOG_H

#include <QDialog>
#include <QAbstractItemModel>

#include "PsiCon.h"
#include "mucutility.h"

namespace Ui {
    class MUCJoinDialog;
}

class MUCJoinDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MUCJoinDialog(QWidget *parent = 0);
	MUCJoinDialog(PsiCon *, PsiAccount *);
    ~MUCJoinDialog();

private slots:
	void showOccupantsChanged(int );
	void updateIdentity(PsiAccount*);
	void serverListBrowse();
	void receivedMUCService(QString host);

private:
    Ui::MUCJoinDialog *ui;

	PsiCon *controller_;
	PsiAccount *account_;

	MUCUtility *mutility_;

	void initializeUI();
};

#endif // MUCJOINDIALOG_H
