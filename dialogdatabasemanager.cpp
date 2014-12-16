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

    setInputFieldsEnabled(false);

    databasesList = xmlHandler.getAllDbs();
    Database::cntID = 0;
    for (Database &database: databasesList)
    {
        database.setID(Database::cntID++);
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
    while (isNameAlreayExistingAfterCreatingNewDb(name))
        name = "New Database " + QString::number(dbCnt++);
    Database database(Database::cntID++, name, "", "", "", "", "");
    databasesList.append(database);
    childItem->setText(0, database.getInternalDbName());
    childItem->setData(0, Qt::UserRole, database.getID());
    //Save the internal name so that the name can get restored after changing it when the new name alredy exists
    childItem->setData(1, Qt::UserRole, database.getInternalDbName());
    childItem->setFlags(childItem->flags() | Qt::ItemIsEditable);
    root->addChild(childItem);

}

void DialogDatabaseManager::addRootChildFromLoaded(Database &database)
{
    QTreeWidgetItem *root = ui->treeWidgetDatabases->topLevelItem(0);
    QTreeWidgetItem *childItem = new QTreeWidgetItem;
    childItem->setText(0, database.getInternalDbName());
    childItem->setData(0, Qt::UserRole, database.getID());
    //Save the internal name so that the name can get restored after changing it when the new name alredy exists
    childItem->setData(1, Qt::UserRole, database.getInternalDbName());
    root->addChild(childItem);
    childItem->setFlags(childItem->flags() | Qt::ItemIsEditable);
}

bool DialogDatabaseManager::isNameAlreayExistingAfterCreatingNewDb(QString name)
{
    for (Database database: databasesList)
    {
        if (name == database.getInternalDbName())
            return true;
    }
    return false;
}

void DialogDatabaseManager::setInputFieldsEnabled(bool status)
{
    //Make the fiels enabled/ unenabled which are associated with the database (input fields, delete btn, connect btn usw)
    ui->pushBtnConnect->setEnabled(status);
    ui->pushBtnDeleteDb->setEnabled(status);
    for(int i = 0; i < ui->formLayoutData->count(); i++)
        ui->formLayoutData->itemAt(i)->widget()->setEnabled(status);
    for (int i = 0; i < ui->verticalLayoutComment->count(); i++)
        ui->verticalLayoutComment->itemAt(i)->widget()->setEnabled(status);
}

void DialogDatabaseManager::clearInputFields()
{
    ui->lineEdDbName->setText("");
    ui->lineEdHostName->setText("");
    ui->lineEdUserName->setText("");
    ui->lineEdPassword->setText("");
    ui->textEdComment->setText("");
}

bool DialogDatabaseManager::matchesTextWithDatabasesListInternalName(const QString text, int crnID)
{
    for (Database database: databasesList)
    {
        // The second statement is important, withit it can be checked if the name is the name
        // of the new object and there the logical must match.
        if (text == database.getInternalDbName() && crnID != database.getID())
            return true;
    }
    return false;
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

void DialogDatabaseManager::on_pushBtnConnect_clicked()
{
    //Save actual data in the input field in the database class object
    QTreeWidgetItem *item = ui->treeWidgetDatabases->currentItem();

    int crnItemID = item->data(0, Qt::UserRole).toInt();
    Database &database = databasesList[getDbPositionInDbList(crnItemID)];
    QString dbName = ui->lineEdDbName->text();
    QString hostName = ui->lineEdHostName->text();
    QString userName = ui->lineEdUserName->text();
    QString password = ui->lineEdPassword->text();
    QString comment = ui->textEdComment->toPlainText();
    database.setDbName(dbName);
    database.setHostName(hostName);
    database.setUserName(userName);
    database.setPasswort(password);
    database.setComment(comment);

    xmlHandler.saveAllDbs(databasesList);
    emit connectToDB(database);
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
    // Show the data from the current database if its not the top level item
    // If it is the top level item clear the input fiels and make then not useable
    if (current != ui->treeWidgetDatabases->topLevelItem(0))
    {
        setInputFieldsEnabled(true);
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
    else
    {
        clearInputFields();
        setInputFieldsEnabled(false);
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
    //Save actual data in the input field in the database class object
    QTreeWidgetItem *item = ui->treeWidgetDatabases->currentItem();

    int crnItemID = item->data(0, Qt::UserRole).toInt();
    Database &database = databasesList[getDbPositionInDbList(crnItemID)];
    QString dbName = ui->lineEdDbName->text();
    QString hostName = ui->lineEdHostName->text();
    QString userName = ui->lineEdUserName->text();
    QString password = ui->lineEdPassword->text();
    QString comment = ui->textEdComment->toPlainText();
    database.setDbName(dbName);
    database.setHostName(hostName);
    database.setUserName(userName);
    database.setPasswort(password);
    database.setComment(comment);

    xmlHandler.saveAllDbs(databasesList);
}



void DialogDatabaseManager::on_treeWidgetDatabases_itemChanged(QTreeWidgetItem *item, int column)
{
    //If the new InternalDbName is already taken, then let the user set a new one.
    QString itemText = item->text(column);
    int crnID = item->data(0, Qt::UserRole).toInt();
    if (matchesTextWithDatabasesListInternalName(itemText, crnID))
    {        
        //Restore the old name before changing it.
        QString oldInternalName = item->data(1, Qt::UserRole).toString();
        item->setText(0, oldInternalName);
        QMessageBox::warning(this, "Name already exists", "It seems like the name for the database already exists. Please choose another one.");
    }
    else
    {
        //Store the new internal name in the item data. So that it can be get restored after a new change when it match with another internal name.
        item->setData(1, Qt::UserRole, itemText);
    }

    qDebug() << "item changed";
}


