#include "mucjoindialog.h"
#include "ui_mucjoindialog.h"

#include "recentandbookmarkedroomsmodel.h"
#include "serverroomlistmodel.h"


MUCJoinDialog::MUCJoinDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MUCJoinDialog)
{
    ui->setupUi(this);
	setWindowFlags(Qt::Window);
}

MUCJoinDialog::MUCJoinDialog(PsiCon *con, PsiAccount *account) :
	QDialog(0),
	ui(new Ui::MUCJoinDialog)
{
	controller_ = con;
	account_ = account;
	mutility_ = NULL;
	ui->setupUi(this);
	setWindowFlags(Qt::Window);
	initializeUI();
}

MUCJoinDialog::~MUCJoinDialog()
{
	if (mutility_) delete mutility_;
    delete ui;
}

void MUCJoinDialog::initializeUI() {
	ui->identityComboBox->setController(controller_);
	ui->identityComboBox->setOnlineOnly(true);
	connect(ui->identityComboBox, SIGNAL(activated(PsiAccount *)), SLOT(updateIdentity(PsiAccount *)));
	connect(ui->publicServerJID, SIGNAL(returnPressed()), SLOT(serverListBrowse()));
	connect(ui->occupantsCheckBox, SIGNAL(stateChanged(int)), SLOT(showOccupantsChanged(int)));
	ui->identityComboBox->setAccount(account_);
	updateIdentity(account_);
}

void MUCJoinDialog::updateIdentity(PsiAccount *account) {
	if (account == NULL) return;
	account_ = account;
	ui->bookmarksList->setModel(new RecentAndBookmarkedRoomsModel(controller_, account));
}

void MUCJoinDialog::serverListBrowse() {
	if (mutility_) delete mutility_;
	mutility_ = new MUCUtility(account_);
	connect(mutility_, SIGNAL(receivedMUCService(QString)), SLOT(receivedMUCService(QString)));
	mutility_->determineMUCServiceForDomain(Jid(ui->publicServerJID->text()));
	/*QAbstractItemModel *model = ui->publicRoomsList->model();
	ui->publicRoomsList->setModel(new ServerRoomListModel(controller_, account_, ui->publicServerJID->text()));
	if (model) delete model;*/
}

void MUCJoinDialog::showOccupantsChanged(int state) {
	ServerRoomListModel *model = qobject_cast<ServerRoomListModel*>(ui->publicRoomsList->model());
	if (model) model->setShowNumberOfOccupants(state == 2 ? true : false);
}

void MUCJoinDialog::receivedMUCService(QString host) {
	fprintf(stderr, "\tresolved to actual MUC component: %s\n", host.toAscii().data());
	if (host == "")
		fprintf(stderr, "\tDidn't find MUC.\n");
	else
		ui->publicServerJID->setText(host);
}

void MUCJoinDialog::receivedListOfRooms(QString host, QList<MUCUtility::MUCRoom> roomList) {

}
