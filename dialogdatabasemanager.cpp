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
    databasesList = xmlHandler.getAllDbs();
    Database::cntID = 0;
    for (Database &database: databasesList)
    {
        database.setID(Database::cntID++);
        for (Database database2: databasesList)
            qDebug() << database2.getID();
        addRootChildFromLoaded(database);
    }


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

void DialogDatabaseManager::addRootChildFromLoaded(Database &database)
{
    QTreeWidgetItem *root = ui->treeWidgetDatabases->topLevelItem(0);
    QTreeWidgetItem *childItem = new QTreeWidgetItem;
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
    // First delete the item and after that remove the database from the database,
    // because else there is an out of range error.
    // Because the after deleting the current item the treewidget select another item and
    // call currentItemChanged. In the slot the function want to load the data from the previous item
    // and search for the item id in the databseslist. But the there is no match an getDbPositionInDbList() returns -1
    // and this result in an error.
    delete currentItem;
    databasesList.removeAt(getDbPositionInDbList(crnItemID));

}

void DialogDatabaseManager::on_treeWidgetDatabases_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    qDebug() << "current item changed";
    // Save the data in the fields in the database class if the previous item is not 0
    if (previous && previous != ui->treeWidgetDatabases->topLevelItem(0))
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

void DialogDatabaseManager::on_buttonBox_accepted()
{
    xmlHandler.saveAllDbs(databasesList);
}
