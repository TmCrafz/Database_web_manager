#include <QtSql/QSqlDatabase>
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lineEdPasswordQui->setEchoMode(QLineEdit::Password);


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
    }


    qDebug() << QSqlDatabase::drivers();

}
