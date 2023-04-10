#ifndef WEATHERREPORT_H
#define WEATHERREPORT_H

#include <QDebug>

#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>
#include <QJsonArray>
#include <QString>
#include <QVector>

#include "observer.h"

/**
 * @brief The WeatherReport class
 * @author FJa
 */
class WeatherReport : public Observable
{
private:
    QString main,description; //> text report
    double temp,temp_min,temp_max; //> temperatures
    double lon,lat; //> localisation

    QVector <Observer*> observers;
public:
    WeatherReport();

    // getters
    const QString& getDescription() const {return description;}
    double getTemp() const {return temp;}
    double getTempMin() const {return temp_min;}
    double getTempMax() const {return temp_max;}
    double getLon() const {return lon;}
    double getLat() const {return lat;}
    //setter
    void setDatas(QString main, QString desc, double temp_, double temp_min_, double temp_max_, double lon_, double lat_);

    //Observers
    void addObserver(Observer* observer) override;
    void removeObserver(Observer* observer) override;
    void notifyObserver() override;
};  

#endif // WEATHERREPORT_H
