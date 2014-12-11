#include "dialogdatabasemanager.h"
#include "ui_dialogdatabasemanager.h"

DialogDatabaseManager::DialogDatabaseManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDatabaseManager)
{
    ui->setupUi(this);

    ui->treeWidgetDatabases->header()->close();

    ui->lineEdPassword->setEchoMode(QLineEdit::Password);
}

DialogDatabaseManager::~DialogDatabaseManager()
{
    delete ui;
}

void DialogDatabaseManager::on_pushBtnNewDb_clicked()
{
    QTreeWidgetItem *root = new QTreeWidgetItem(ui->treeWidgetDatabases->itemAt(0, 0));
    addChildToTree(root, "New database");
}

void DialogDatabaseManager::addChildToTree(QTreeWidgetItem *parent, QString name)
{
    parent->setText(0, name);
}
