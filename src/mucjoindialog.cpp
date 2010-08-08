#include "mucjoindialog.h"
#include "ui_mucjoindialog.h"

#include "recentandbookmarkedroomsmodel.h"
#include "serverroomlistmodel.h"
#include "jidvalidator.h"
#include "mucjoindlg.h"

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
	connect(ui->cancelButton, SIGNAL(clicked()), SLOT(cancelButtonClicked()));
	connect(ui->joinButton, SIGNAL(clicked()), SLOT(joinButtonClicked()));
	ui->publicServerJID->setValidator(new JidValidator());
	ui->roomJIDLineEdit->setValidator(new JidValidator());
	ui->identityComboBox->setAccount(account_);
	updateIdentity(account_);
	ui->publicServerJID->setText(account_->jid().domain());
	serverListBrowse();
}

void MUCJoinDialog::updateIdentity(PsiAccount *account) {
	if (account == NULL) return;
	account_ = account;
	ui->bookmarksList->setModel(new RecentAndBookmarkedRoomsModel(controller_, account));
}

void MUCJoinDialog::serverListBrowse() {
	if (mutility_) delete mutility_;
	mutility_ = new MUCUtility(account_);
	mutility_->setProgressWidget(ui->processBar);
	connect(mutility_, SIGNAL(receivedMUCService(Jid)), SLOT(receivedMUCService(Jid)));
	mutility_->determineMUCServiceForDomain(Jid(ui->publicServerJID->text()));
}

void MUCJoinDialog::cancelButtonClicked() {
	close();
}

void MUCJoinDialog::joinButtonClicked() {
	QList<Jid> rooms_to_join;
	fprintf(stderr, "\tjoinButtonClicked()\n");

	// check manual enter field
	if (ui->roomJIDLineEdit->hasAcceptableInput()) {
		rooms_to_join.append(Jid(ui->roomJIDLineEdit->text()));
	}

	// check bookmark/history view
	QModelIndexList selected_bookmarks = ui->bookmarksList->selectionModel()->selectedRows();
	RecentAndBookmarkedRoomsModel *model_book =
			qobject_cast<RecentAndBookmarkedRoomsModel*>(ui->bookmarksList->model());
	if (model_book) rooms_to_join << model_book->getJidListForModelIndexList(selected_bookmarks);

	// check server room view
	QModelIndexList selected_serverrooms = ui->publicRoomsList->selectionModel()->selectedRows();
	ServerRoomListModel *model_serverrooms =
			qobject_cast<ServerRoomListModel*>(ui->publicRoomsList->model());
	if (model_serverrooms) rooms_to_join << model_serverrooms->getJidListForModelIndexList(selected_serverrooms);

	// join rooms - in a non-disturbing away
	QString nickname = ui->nicknameLineEdit->text();
	foreach( Jid jid, rooms_to_join) {
		if (jid.resource() == "") jid = jid.withResource(nickname);
		MUCJoinDlg* w = new MUCJoinDlg(controller_, account_);

		w->setJid(jid);
		w->setNick(jid.resource());

		w->setAutoHiding();
		w->doJoin();
	}
	// close dialog
	close();
}

void MUCJoinDialog::showOccupantsChanged(int state) {
	ServerRoomListModel *model = qobject_cast<ServerRoomListModel*>(ui->publicRoomsList->model());
	if (model) model->setShowNumberOfOccupants(state == 2 ? true : false);
}

void MUCJoinDialog::receivedMUCService(Jid host) {
	if (host == "")
		fprintf(stderr, "\tDidn't find MUC.\n");
	else
		ui->publicServerJID->setText(host.full());
		QAbstractItemModel *oldModel = ui->publicRoomsList->model();
		ServerRoomListModel *model = new ServerRoomListModel(controller_, account_, host.full());
		model->setShowNumberOfOccupants(ui->occupantsCheckBox->checkState() == 2 ? true : false);
		ui->publicRoomsList->setModel(model);
		if (oldModel) delete oldModel;
		model->setProgressBar(ui->processBar);
}
