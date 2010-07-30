#include "mucjoindialog.h"
#include "ui_mucjoindialog.h"

#include "recentandbookmarkedroomsmodel.h"

MUCBookmarkModel::MUCBookmarkModel(QWidget *parent) : QAbstractItemModel(parent) {

}

MUCBookmarkModel::~MUCBookmarkModel() {

}

int MUCBookmarkModel::columnCount(const QModelIndex &parent) const {
	fprintf(stderr, "MUCBookmarkModel::columnCount()\n");
	return 2;
}

int MUCBookmarkModel::rowCount(const QModelIndex & parent) const {
	fprintf(stderr, "MUCBookmarkModel::rowCount()\n");
	// return number of bookmark items
	return 0;
}

QVariant MUCBookmarkModel::headerData(int section, Qt::Orientation orientation, int role) const {
	fprintf(stderr, "MUCBookmarkModel::headerData()\n");
	if (role != Qt::DisplayRole) return QVariant();
	if (section == 0) return QVariant("Name");
	if (section == 1) return QVariant("Room JID");
	return QVariant();
}

QVariant MUCBookmarkModel::data(const QModelIndex &index, int role) const {
	fprintf(stderr, "MUCBookmarkModel::data()\n");
	if (role != Qt::DisplayRole) return QVariant();
	if (index.row() == 0) {
		return QVariant("Hello");
	} else {
		return QVariant();
	}
}

QModelIndex MUCBookmarkModel::index(int row, int column, const QModelIndex &parent) const {
	fprintf(stderr, "MUCBookmarkModel::index()\n");
	return createIndex(row, column, 0);
}

QModelIndex MUCBookmarkModel::parent(const QModelIndex &index) const {
	fprintf(stderr, "MUCBookmarkModel::parent()\n");
	return QModelIndex();
}

MUCJoinDialog::MUCJoinDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MUCJoinDialog)
{
    ui->setupUi(this);
	setWindowFlags(Qt::Window);
	MUCBookmarkModel *bmodel = new MUCBookmarkModel();
	ui->bookmarksList->setModel(bmodel);
}

MUCJoinDialog::MUCJoinDialog(PsiCon *con, PsiAccount *account) :
	QDialog(0),
	ui(new Ui::MUCJoinDialog)
{
	controller_ = con;
	account_ = account;
	ui->setupUi(this);
	setWindowFlags(Qt::Window);
	MUCBookmarkModel *bmodel = new MUCBookmarkModel();
	ui->bookmarksList->setModel(bmodel);
	initializeUI();
}

MUCJoinDialog::~MUCJoinDialog()
{
    delete ui;
}

void MUCJoinDialog::initializeUI() {
	ui->identityComboBox->setController(controller_);
	ui->identityComboBox->setOnlineOnly(true);
	connect(ui->identityComboBox, SIGNAL(activated(PsiAccount *)), SLOT(updateIdentity(PsiAccount *)));
	ui->identityComboBox->setAccount(account_);
	updateIdentity(account_);
}

void MUCJoinDialog::updateIdentity(PsiAccount *account) {
	if (account == NULL) return;
	ui->bookmarksList->setModel(new RecentAndBookmarkedRoomsModel(controller_, account));
}
