#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionDatabase_manager_triggered()
{
    mDialogDatabaseManager = new DialogDatabaseManager(0);
    connect(mDialogDatabaseManager, SIGNAL(connectToDB(QString)), this, SLOT(connectWithDb(QString)));
    mDialogDatabaseManager->show();
}

void MainWindow::connectWithDb(QString internalDbName)
{
    qDebug() << "Connect to db: " + internalDbName;

}
