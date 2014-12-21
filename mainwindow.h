#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dialogdatabasemanager.h"
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void clearAllShownDbFields();
    void loadTableEnries(const QString TableName);
private slots:
    void on_actionDatabase_manager_triggered();
    void connectWithDb(Database &database);

    void on_pushBtnAddEntryFc_clicked();

    void on_treeWidgetLoadedTables_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_pushBtnConnectQui_clicked();

private:
    Ui::MainWindow *ui;

    DialogDatabaseManager *mDialogDatabaseManager;
};

#endif // MAINWINDOW_H
