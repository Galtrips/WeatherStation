#include "dbmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

DbManager::DbManager(const QString &path)
{
    sqldb = QSqlDatabase::addDatabase("QSQLITE");
    sqldb.setDatabaseName(path);

    if (!sqldb.open())
    {
        qDebug() << "Error: connection with database fail";
    }
    else
    {
        qDebug() << "Database: connection ok";
    }
}

DbManager::~DbManager()
{
    if (sqldb.isOpen())
    {
        sqldb.close();
    }
}

bool DbManager::isOpen() const
{
    return sqldb.isOpen();
}

bool DbManager::createTable()
{
    bool success = false;

    QSqlQuery query;
    query.prepare("CREATE TABLE pollution(id INTEGER PRIMARY KEY, dt INTEGER, aqi INTEGER);");

    if (!query.exec())
    {
        qDebug() << "Couldn't create the table 'pollution': one might already exist.";
        success = false;
    }

    return success;
}

bool DbManager::addData(int dt, int aqi) {

    bool success = false;

    QSqlQuery queryAdd;
    queryAdd.prepare("INSERT INTO pollution (dt,aqi) VALUES (:dt,:aqi)");
    queryAdd.bindValue(":dt", dt);
    queryAdd.bindValue(":aqi", aqi);

    if(queryAdd.exec())
    {
        success = true;
        this->notifyObserver();
    }
    else
    {
        qDebug() << "add dt/aqi failed: " << queryAdd.lastError();
    }

    return success;
}

bool DbManager::removeData(int dt)
{
    bool success = false;

    if (entryExists(dt))
    {
        QSqlQuery queryDelete;
        queryDelete.prepare("DELETE FROM pollution WHERE dt = (:dt)");
        queryDelete.bindValue(":dt", dt);
        success = queryDelete.exec();

        if(!success)
        {
            qDebug() << "remove data failed: " << queryDelete.lastError();
        }
    }
    else
    {
        qDebug() << "remove data failed: dt doesnt exist";
    }

    return success;
}

void DbManager::printAllData() const
{
    qDebug() << "Data in db:";
    QSqlQuery query("SELECT * FROM pollution");
    int idDt = query.record().indexOf("dt");
    int idAqi = query.record().indexOf("aqi");
    while (query.next())
    {
        int dt = query.value(idDt).toInt();
        int aqi = query.value(idAqi).toInt();
        qDebug() << "===" << dt << " " << aqi;
    }
}

bool DbManager::entryExists(int dt) const
{
    bool exists = false;

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT dt FROM pollution WHERE dt = (:dt)");
    checkQuery.bindValue(":dt", dt);

    if (checkQuery.exec())
    {
        if (checkQuery.next())
        {
            exists = true;
        }
    }
    else
    {
        qDebug() << "data exists failed: " << checkQuery.lastError();
    }

    return exists;
}

bool DbManager::removeAllData()
{
    bool success = false;

    QSqlQuery removeQuery;
    removeQuery.prepare("DELETE FROM pollution");

    if (removeQuery.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "remove all data failed: " << removeQuery.lastError();
    }

    return success;
}

void DbManager::addObserver(Observer* observer) {
    this->observers.push_back(observer);
}

void DbManager::removeObserver(Observer* oldObserver) {
    int cpt = 0;
    for (auto observer : this->observers) {
        if (observer == oldObserver) {
            this->observers.remove(cpt);
            break;
        }
        cpt++;
    }
}

void DbManager::notifyObserver() {
    for (auto observer : this->observers) {
        observer->update();
    }
}

QVector <std::array<int, 2>> DbManager::getAllData() const {

    QVector <std::array<int, 2>> tab;

    QDateTime currentdt = QDateTime::currentDateTime();
    qint64 msdt = currentdt.toMSecsSinceEpoch();
    msdt = msdt / 1000;

    QSqlQuery query;

    query.prepare("SELECT * FROM pollution");
    query.exec();

    int idDt = query.record().indexOf("dt");
    int idAqi = query.record().indexOf("aqi");

    std::array<int, 2> value;

    while (query.next())
    {
        int dt = query.value(idDt).toInt();
        int aqi = query.value(idAqi).toInt();
        value[0] = dt;
        value[1] = aqi;
        tab.append(value);
    }

    return tab;
}