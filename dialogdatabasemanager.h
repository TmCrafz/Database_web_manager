#ifndef DIALOGDATABASEMANAGER_H
#define DIALOGDATABASEMANAGER_H

#include <QDialog>
#include <QTreeWidget>

namespace Ui {
class DialogDatabaseManager;
}

class DialogDatabaseManager : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDatabaseManager(QWidget *parent = 0);
    ~DialogDatabaseManager();

    void addChildToTree(QTreeWidgetItem *parent, QString name);
private slots:
    void on_pushBtnNewDb_clicked();

private:
    Ui::DialogDatabaseManager *ui;
};

#endif // DIALOGDATABASEMANAGER_H
