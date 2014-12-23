#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    loadingPhase(false)
{
    ui->setupUi(this);

    ui->lineEdPasswordQui->setEchoMode(QLineEdit::Password);
    ui->splitter->setStretchFactor(1, 1);
    ui->treeWidgetLoadedTables->header()->close();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionDatabase_manager_triggered()
{
    mDialogDatabaseManager = new DialogDatabaseManager(0);
    connect(mDialogDatabaseManager, SIGNAL(connectToDB(Database&)), this, SLOT(connectWithDb(Database&)));
    mDialogDatabaseManager->show();
}

void MainWindow::connectWithDb(Database &database)
{
    loadingPhase = true;
    qDebug() << "Connect to db: " << database.getInternalDbName();
    qDebug() << "Db name: " << database.getDbName();
    qDebug() << "Hostname " << database.getHostName();
    qDebug() << "Username " << database.getUserName();
    qDebug() << "Password: " << database.getPasswort();
    qDebug() << "Commment: " << database.getComment();

    ui->textBrowserStatus->append("Connecting to database " + database.getInternalDbName() + "...");
    ui->textBrowserStatus->append("Database name: " + database.getDbName());
    ui->textBrowserStatus->append("Hostname " + database.getHostName());
    ui->textBrowserStatus->append("Username " + database.getUserName());

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(database.getHostName());
    db.setDatabaseName(database.getDbName());
    db.setUserName(database.getUserName());
    db.setPassword(database.getPasswort());
    if(!db.open())
    {
        qDebug() << "Cant connect with db";
        ui->textBrowserStatus->append("<font color = red>Cannot connect with databank (wrong password?)</font>");
    }
    else
    {
        qDebug() << "Connected with databank";
        ui->textBrowserStatus->append("<font color = green>Connected with databank</font>");
        QStringList tables = db.tables(QSql::Tables);
        //Clear current treeWidget items when existing
        ui->treeWidgetLoadedTables->topLevelItem(0)->takeChildren();
        //Add tables to treeWidget
        ui->textBrowserStatus->append("Tables found:");
        for (QString table : tables)
        {
            ui->textBrowserStatus->append(table);
            QTreeWidgetItem *item = new QTreeWidgetItem();
            item->setText(0, table);
            ui->treeWidgetLoadedTables->topLevelItem(0)->addChild(item);
        }
        //Choose the first child item
        if (tables.size() > 0)
        {
            QTreeWidgetItem *firstChildItem = ui->treeWidgetLoadedTables->topLevelItem(0)->child(0);
            ui->treeWidgetLoadedTables->setCurrentItem(firstChildItem);

        }
    }

    loadingPhase = false;
}

void MainWindow::clearAllShownDbFields()
{
    ui->tableWidgetDbTableEntries->clear();
    ui->tableWidgetDbTableEntries->setRowCount(0);
    ui->tableWidgetDbTableEntries->setColumnCount(0);
}

void MainWindow::loadTableEnries(const QString TableName)
{
    loadingPhase = true;
    QSqlQuery query;

    if (!query.exec(QString("SELECT * FROM " + TableName)))
    {
        ui->textBrowserStatus->append("<font color = red>Cannot open table: " + TableName + "</font>");
    }
    else
    {
        ui->textBrowserStatus->append("<font color = green>Opend table : " + TableName + "</font>");
    }
    QSqlRecord rec = query.record();
    //Load the header of the db table widget
    int clmCnt = rec.count();
    ui->textBrowserStatus->append("found number of columns: " + QString::number(clmCnt));
    ui->tableWidgetDbTableEntries->setColumnCount(clmCnt);
    QStringList columnsList;
    for (int i = 0; i != clmCnt; i++)
    {
        columnsList.append(rec.fieldName(i));
        ui->tableWidgetDbTableEntries->setHorizontalHeaderItem(i, new QTableWidgetItem(rec.fieldName(i)));        
    }
    ui->textBrowserStatus->append("found columns:");
    for (auto columnName : columnsList)
        ui->textBrowserStatus->append(columnName);
    //Load the entries
    int rowCnt = 0;
    while(query.next())
    {
        ui->tableWidgetDbTableEntries->insertRow(rowCnt);
        for (int i = 0; i != clmCnt; i++)
        {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(i).toString());
            ui->tableWidgetDbTableEntries->setItem(rowCnt, i, item);
            // The first item in the row shouldnt be ditable because its normally the ID
            // and the id is the only 100% identification possibility.
            if(i == 0)
                item->setFlags(item->flags() ^ Qt::ItemIsEditable);
        }
        rowCnt++;
    }
    loadingPhase = false;
}

void MainWindow::on_pushBtnAddEntryFc_clicked()
{
    QSqlDatabase db = QSqlDatabase::database();
    QStringList tables = db.tables(QSql::Tables);
    ui->textBrowserStatus->append("Tables:");
    for (QString table : tables)
        ui->textBrowserStatus->append(table);

    QSqlQuery query;
    // Load table
    if (!query.exec(QString("SELECT * FROM " + tables.at(0))))
    {
        ui->textBrowserStatus->append("<font color = red>Cannot open table: " + tables.at(0) + "</font>");
    }
    else
    {
        ui->textBrowserStatus->append("<font color = green>Opend table : " + tables.at(0) + "</font>");
    }

    QSqlRecord rec = query.record();
    int clmCnt = rec.count();
    ui->textBrowserStatus->append("Number of columns: " + QString::number(clmCnt));
    QStringList columnsList;
    for (int i = 0; i != clmCnt; i++)
        columnsList.append(rec.fieldName(i));
    ui->textBrowserStatus->append("column names:");

    //Add Entry
    QString title = ui->lineEdTitleFc->text();
    QString content = ui->plainTextEdContentFc->toPlainText();
    QDateTime currentTime = QDateTime::currentDateTime();

    query.prepare("INSERT INTO " + tables.at(0) + " (" + columnsList.at(1) + ", " + columnsList.at(2) + ", " + columnsList.at(3) + ") "
                  "VALUES (?, ?, ?)");
    query.addBindValue(title);
    query.addBindValue(currentTime);
    query.addBindValue(content);
    query.exec();
    // If the current selected table is not the TopLevelItem reload the data in the table
    if (ui->treeWidgetLoadedTables->currentItem() != ui->treeWidgetLoadedTables->topLevelItem(0))
    {
        const QString selectedTableName = ui->treeWidgetLoadedTables->currentItem()->text(0);
        clearAllShownDbFields();
        loadTableEnries(selectedTableName);
    }
}

void MainWindow::on_treeWidgetLoadedTables_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    //Load the data of the selected table if the current item is not the top level item
    clearAllShownDbFields();
    if (current != ui->treeWidgetLoadedTables->topLevelItem(0))
    {
        const QString TableName = current->text(0);
        loadTableEnries(TableName);
    }


}

void MainWindow::on_pushBtnConnectQui_clicked()
{
    QString dbName = ui->lineEdDbNameQui->text();
    QString hostName = ui->lineEdHostNameQui->text();
    QString userName = ui->lineEdUserNameQui->text();
    QString password = ui->lineEdPasswordQui->text();
    Database database(0, "Quick_connect", dbName, hostName, userName, password, "");
    connectWithDb(database);
}

void MainWindow::on_tableWidgetDbTableEntries_itemChanged(QTableWidgetItem *item)
{
    // Update the field in the db which was edit in the table widget, but only when the programm is not
    // loading the data (because then tablewidget calls itemChanged() too).
    if (!loadingPhase)
    {
        qDebug() << "itemChanged";
        QString currnetTable = ui->treeWidgetLoadedTables->currentItem()->text(0);
        QString headerName = ui->tableWidgetDbTableEntries->horizontalHeaderItem(item->column())->text();
        qDebug() << "Item row " << item->row();

        QString id = ui->tableWidgetDbTableEntries->item(item->row(), 0)->text();
        QString updateField = "UPDATE " + currnetTable + " SET " + headerName + "=" + "'" + item->text() + "' " +
                "WHERE " + ui->tableWidgetDbTableEntries->horizontalHeaderItem(0)->text() + "=" + "'" + id + "'";
        ui->textBrowserStatus->append(updateField);
        QSqlQuery query;
        if(!query.exec(updateField))
            ui->textBrowserStatus->append("<font color = red>Cannot update row</font>");
        else
            ui->textBrowserStatus->append("<font color = green>Updatet row</font>");
    }

}


