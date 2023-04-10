#include "weatherreport.h"
#include <QDateTime>
#include <QTimeZone>
//#include <unistd.h>

WeatherReport::WeatherReport()
    : temp(0.0),temp_min(0.0),temp_max(0.0),
      lon(0.0),lat(0.0)
{ }

void WeatherReport::setDatas(QString main, QString desc, double temp_, double temp_min_, double temp_max_, double lon_, double lat_) {
    this->main = main;
    this->description = desc;
    this->temp = temp_;
    this->temp_min = temp_min_;
    this->temp_max = temp_max_;
    this->lon = lon_;
    this->lat = lat_;

    this->notifyObserver();
}

void WeatherReport::addObserver(Observer* observer) {
    this->observers.push_back(observer);
}

void WeatherReport::removeObserver(Observer* oldObserver) {
    int cpt = 0;
    for (auto observer : this->observers) {
        if (observer == oldObserver) {
            this->observers.remove(cpt);
            break;
        }
        cpt++;
    }
}

void WeatherReport::notifyObserver() {
    for (auto observer : this->observers) {
        observer->update();
    }
}