#ifndef DIALOGDATABASEMANAGER_H
#define DIALOGDATABASEMANAGER_H

#include <QDialog>
#include <QTreeWidget>
#include <QString>
#include <QMessageBox>
#include "xmlhandler.h"

namespace Ui {
class DialogDatabaseManager;
}

class DialogDatabaseManager : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDatabaseManager(QWidget *parent = 0);
    ~DialogDatabaseManager();

    void addRootChild();
    void addRootChildFromLoaded(Database &database);
    bool isNameAlreayExistingAfterCreatingNewDb(QString name);
    void setInputFieldsEnabled(bool status);
    void clearInputFields();
    bool matchesTextWithDatabasesListInternalName(const QString text, int crnID);
    int getDbPositionInDbList(int crnItemID);

signals:
    void connectToDB(Database &database);
private slots:


    void on_pushBtnNewDb_clicked();

    void on_pushBtnDeleteDb_clicked();

    void on_treeWidgetDatabases_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_buttonBox_accepted();


    void on_treeWidgetDatabases_itemChanged(QTreeWidgetItem *item, int column);

    void on_pushBtnConnect_clicked();

private:
    int dbCnt = 0;

    Ui::DialogDatabaseManager *ui;
    XmlHandler xmlHandler;
    QList<Database> databasesList;
};

#endif // DIALOGDATABASEMANAGER_H
