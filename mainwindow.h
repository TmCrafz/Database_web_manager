#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dialogdatabasemanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionDatabase_manager_triggered();
    void connectWithDb(Database &database);

private:
    Ui::MainWindow *ui;

    DialogDatabaseManager *mDialogDatabaseManager;
};

#endif // MAINWINDOW_H
