#include "dialogdatabasemanager.h"
#include "ui_dialogdatabasemanager.h"

DialogDatabaseManager::DialogDatabaseManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDatabaseManager)
{
    ui->setupUi(this);

    ui->treeWidgetDatabases->header()->close();

    ui->lineEdPassword->setEchoMode(QLineEdit::Password);

    //teeWidgetStartSelection();


}

DialogDatabaseManager::~DialogDatabaseManager()
{
    delete ui;
}

void DialogDatabaseManager::teeWidgetStartSelection()
{
    qDebug() << "startselection  ";
    for(int i = 0; i < ui->formLayoutData->count(); i++)
        ui->formLayoutData->itemAt(i)->widget()->setEnabled(false);
    for (int i = 0; i < ui->verticalLayoutComment->count(); i++)
        ui->verticalLayoutComment->itemAt(i)->widget()->setEnabled(false);
    QTreeWidgetItem *root = new QTreeWidgetItem(ui->treeWidgetDatabases->itemAt(0, 0));
    QList<QString> databaseNames = xmlHandler.getAllDbNames();
    QList<QTreeWidgetItem *>items;
    qDebug() << "databasenames size " << databaseNames.size();
    for (QString name : databaseNames)
        root->setText(0, name);


    /*
    QTreeWidgetItem *root = new QTreeWidgetItem(ui->treeWidgetDatabases->itemAt(0, 0));
    // If there are databases saved then they get loaded and select one in the right menu
    // else deactivate all input fields

    if(root->childCount() != 0)
    {
        //Load the saved database names and add them to the tree widget
        QTreeWidgetItem *root = new QTreeWidgetItem(ui->treeWidgetDatabases->itemAt(0, 0));
        QList<QString> databaseNames = xmlHandler.getAllDbNames();
        QList<QTreeWidgetItem *>items;
        qDebug() << "databasenames size " << databaseNames.size();
        for (QString name : databaseNames)
            root->setText(0, name);
    }
    else
    {
        //deactivate all input fields
        for(int i = 0; i < ui->formLayoutData->count(); i++)
            ui->formLayoutData->itemAt(i)->widget()->setEnabled(false);
        for (int i = 0; i < ui->verticalLayoutComment->count(); i++)
            ui->verticalLayoutComment->itemAt(i)->widget()->setEnabled(false);
    }
    */
}

void DialogDatabaseManager::on_pushBtnNewDb_clicked()
{
    addChildToTree(ui->treeWidgetDatabases->topLevelItem(0), "New database");
}

void DialogDatabaseManager::addChildToTree(QTreeWidgetItem *parent, QString name)
{   
    QTreeWidgetItem *db = new QTreeWidgetItem();
    db->setText(0, name);
    parent->addChild(db);
}




void DialogDatabaseManager::on_treeWidgetDatabases_itemSelectionChanged()
{
    // If the selected item is the root item then make the input fields not useable.
    // Else make then useable.
    QTreeWidgetItem *root = ui->treeWidgetDatabases->topLevelItem(0);
    if (!root->isSelected())
    {
        for(int i = 0; i < ui->formLayoutData->count(); i++)
            ui->formLayoutData->itemAt(i)->widget()->setEnabled(true);
        for (int i = 0; i < ui->verticalLayoutComment->count(); i++)
            ui->verticalLayoutComment->itemAt(i)->widget()->setEnabled(true);
    }
    else
    {
        for(int i = 0; i < ui->formLayoutData->count(); i++)
            ui->formLayoutData->itemAt(i)->widget()->setEnabled(false);
        for (int i = 0; i < ui->verticalLayoutComment->count(); i++)
            ui->verticalLayoutComment->itemAt(i)->widget()->setEnabled(false);
    }
}
