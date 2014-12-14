#ifndef DATABASE_H
#define DATABASE_H
#include <QString>

class Database
{
private:
    int mID;
    QString mInternalDbName;
    QString mDbName;
    QString mHostName;
    QString mUserName;
    QString mPasswort;
    QString mComment;

public:
    static int cntID;

    Database(int ID, QString internalDbName, QString dbName, QString hostName, QString userName, QString passwort, QString comment);

    int getID();
    QString getInternalDbName() const;
    QString getDbName();
    QString getHostName();
    QString getUserName();
    QString getPasswort();
    QString getComment();

    void setID(int ID);
    void setInternalDbName(QString internalDbName);
    void setDbName(QString dbName);
    void setHostName(QString hostName);
    void setUserName(QString userName);
    void setPasswort(QString password);
    void setComment(QString comment);

};

#endif // DATABASE_H
