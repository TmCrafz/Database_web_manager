#include <QMessageBox>
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

void DialogDatabaseManager::addRootChild()
{
    QTreeWidgetItem *root = ui->treeWidgetDatabases->topLevelItem(0);
    QTreeWidgetItem *childItem = new QTreeWidgetItem;
    QString name("New Database " + QString::number(dbCnt++));
    Database database(Database::cntID++, name, "", "", "", "", "");
    databasesList.append(database);
    childItem->setText(0, database.getInternalDbName());
    childItem->setData(0, Qt::UserRole, database.getID());
    root->addChild(childItem);
}


void DialogDatabaseManager::on_pushBtnNewDb_clicked()
{
    addRootChild();
}

void DialogDatabaseManager::on_pushBtnDeleteDb_clicked()
{
    QTreeWidgetItem *currentItem = ui->treeWidgetDatabases->currentItem();
    int crnItemID = currentItem->data(0, Qt::UserRole).toInt();
    databasesList.removeAt(getDbPositionInDbList(crnItemID));
    delete currentItem;
}

void DialogDatabaseManager::on_treeWidgetDatabases_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    // Save the data in the fields in the database class if the previous item is not 0
    if (previous)
    {
        int prevItemID = previous->data(0, Qt::UserRole).toInt();
        Database &prevDatabase = databasesList[getDbPositionInDbList(prevItemID)];
        QString prevDbName = ui->lineEdDbName->text();
        QString prevHostName = ui->lineEdHostName->text();
        QString prevUserName = ui->lineEdUserName->text();
        QString prevPassword = ui->lineEdPassword->text();
        QString prevComment = ui->textEdComment->toPlainText();
        prevDatabase.setDbName(prevDbName);
        prevDatabase.setHostName(prevHostName);
        prevDatabase.setUserName(prevUserName);
        prevDatabase.setPasswort(prevPassword);
        prevDatabase.setComment(prevComment);
    }
    // Show the data from the current database if its the top level item
    if (current != ui->treeWidgetDatabases->topLevelItem(0))
    {
        int crnItemID = current->data(0, Qt::UserRole).toInt();
        Database &crnDatabase = databasesList[getDbPositionInDbList(crnItemID)];
        QString crnDbName = crnDatabase.getDbName();
        QString crnHostName = crnDatabase.getHostName();
        QString crnUserName = crnDatabase.getUserName();
        QString crnPassword = crnDatabase.getPasswort();
        QString crnComment = crnDatabase.getComment();
        ui->lineEdDbName->setText(crnDbName);
        ui->lineEdHostName->setText(crnHostName);
        ui->lineEdUserName->setText(crnUserName);
        ui->lineEdPassword->setText(crnPassword);
        ui->textEdComment->setText(crnComment);
    }


}

int DialogDatabaseManager::getDbPositionInDbList(int currentItemID){
    for (int i = 0; i < databasesList.size(); i++)
    {
        Database database = databasesList.at(i);
        if (currentItemID == database.getID())
            return i;
    }
    return -1;
}
