#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TP5_WeatherStation.h"
#include "weatherreport.h"
#include "dbmanager.h"
#include "viewreport.h"
#include "viewpollution.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>

QT_BEGIN_NAMESPACE
namespace Ui { class TP5_WeatherStationClass; }
QT_END_NAMESPACE

/**
 * @brief The TP5_WeatherStation class
 * @author Fja
 */
class TP5_WeatherStation : public QMainWindow
{
    Q_OBJECT

public:
    TP5_WeatherStation(DbManager *dbm, QWidget* parent = nullptr);
    ~TP5_WeatherStation();

private:
    Ui::TP5_WeatherStationClass* ui;

    WeatherReport* weatherReport;
    DbManager* dbmanager;
    QNetworkAccessManager* netmanager;
    QNetworkAccessManager* pollutionmanager;

    ViewReport *reportView;
    ViewPollution *pollutionView;

public slots:
    void weatherRequest();
    void weatherReplyFinished(QNetworkReply* reply);
    void pollutionRequest();
    void pollutionReplyFinished(QNetworkReply* reply);
};
