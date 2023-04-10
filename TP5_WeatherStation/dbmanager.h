#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>
#include <QJsonObject>
#include <QVector>
#include "observer.h"

/**
 * \class DbManager
 *
 * \brief SQL Database Manager class
 *
 * DbManager sets up the connection with SQL database
 * and performs some basics queries. The class requires
 * existing SQL database. You can create it with sqlite:
 * 1. $ sqlite3 pollution.db
 * 2. sqlite> CREATE TABLE pollution(ids integer primary key, dt interger, aqi integer);
 * 3. sqlite> .quit
 */
class DbManager : public Observable
{
private:
    QSqlDatabase sqldb;

    QVector <Observer*> observers;
public:
    /**
     * @brief Constructor
     *
     * Constructor sets up connection with db and opens it
     * @param path - absolute path to db file
     */
    DbManager(const QString& path);

    /**
     * @brief Destructor
     *
     * Close the db connection
     */
    ~DbManager();

    bool isOpen() const;

    /**
     * @brief Creates a new 'pollution' table if it doesn't already exist
     * @return true - 'pollution' table created successfully, false - table not created
     */
    bool createTable();

    /**
     * @brief Add data to db
     * @param dt - date time
     * @param aqi - air quality indice
     * @return true - data added successfully, false - data not added
     */
    bool addData(int dt, int aqi);

    /**
     * @brief Remove data of dt "dt" from db
     * @param dt - dt of data to remove.
     * @return true - data removed successfully, false - data not removed
     */
    bool removeData(int dt);

    /**
     * @brief Check if data of dt "dt" exists in db
     * @param dt - dt of data to  to check.
     * @return true - data exists, false - data does not exist
     */
    bool entryExists(int dt) const;

    /**
     * @brief Print values of all data in db
     */
    void printAllData() const;

    /**
     * @brief return values of all data in db
     */
    QVector<std::array<int, 2>> getAllData() const;

    /**
     * @brief Remove all data from db
     * @return true - all data removed successfully, false - not removed
     */
    bool removeAllData();

    //Observers
    void addObserver(Observer* observer) override;
    void removeObserver(Observer* observer) override;
    void notifyObserver() override;
};

#endif // DBMANAGER_H
