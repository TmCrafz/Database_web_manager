#include "database.h"

int Database::cntID = 0;

Database::Database(int ID, QString internalDbName, QString dbName, QString hostName, QString userName, QString passwort, QString comment) :
    mID(ID),
    mInternalDbName(internalDbName),
    mDbName(dbName),
    mHostName(hostName),
    mUserName(userName),
    mPasswort(passwort),
    mComment(comment)
{    

}

int Database::getID(){ return mID; }
QString Database::getInternalDbName() const { return mInternalDbName; }
QString Database::getDbName() { return mDbName; }
QString Database::getHostName() { return mHostName; }
QString Database::getUserName() { return mUserName; }
QString Database::getPasswort() { return mPasswort; }
QString Database::getComment() { return mComment; }

void Database::setID(int ID) { mID = ID; }
void Database::setInternalDbName(QString internalDbName) { mInternalDbName = internalDbName; }
void Database::setDbName(QString dbName) { mDbName = dbName; }
void Database::setHostName(QString hostName) { mHostName = hostName; }
void Database::setUserName(QString userName) { mUserName = userName; }
void Database::setPasswort(QString password) { mPasswort = password; }
void Database::setComment(QString comment) { mComment = comment; }
