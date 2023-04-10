//#include <unistd.h>
#include <QtNetwork/QNetworkAccessManager>
#include <QByteArray>

#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>
#include <QJsonArray>
#include <QDateTime>

#include "TP5_WeatherStation.h"
#include "ui_TP5_WeatherStation.h"

#include "weatherreport.h"

TP5_WeatherStation::TP5_WeatherStation(DbManager *dbm, QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::TP5_WeatherStationClass)
    , weatherReport (new WeatherReport) // Weather Data class
    , dbmanager (dbm)                   // DB Manager, for Pollution Data
    , netmanager (nullptr)    
    , pollutionmanager(nullptr)    // NetWork Manager, for http requests
{
    ui->setupUi(this);

    // Weather report View
    reportView = new ViewReport(weatherReport,ui);
    // Pollution Forecast View
    pollutionView = new ViewPollution(dbmanager, ui->groupBox_pollution);

    weatherReport->addObserver(reportView);
    dbmanager->addObserver(pollutionView);

    // netmanager here (or better in initialisation list)  + callback to replyFinished

    netmanager = new QNetworkAccessManager(this);
    pollutionmanager = new QNetworkAccessManager(this);

    connect(netmanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(weatherReplyFinished(QNetworkReply*)));
    connect(pollutionmanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(pollutionReplyFinished(QNetworkReply*)));

    //weatherRequest();
    // uncomment once observable implemented
    connect(ui->pushButton_weather_request, &QPushButton::pressed, this, &TP5_WeatherStation::weatherRequest);
    connect(ui->pushButton_weather_request, &QPushButton::pressed, this, &TP5_WeatherStation::pollutionRequest);

}

TP5_WeatherStation::~TP5_WeatherStation()
{
    delete ui;
    delete dbmanager;
    if (netmanager != nullptr)
        netmanager->deleteLater();
}

void TP5_WeatherStation::weatherRequest() {

    // Pour tester les entêtes HTTP
    QString URL = "https://api.openweathermap.org/data/2.5/weather?q=bourg-en-bresse,fr&units=metric&lang=fr&appid=0f86cd77eba28eed506786b92de7bb40";
    QUrl url(URL);
    QNetworkRequest request;

    request.setUrl(url);
    //--header ’Accept: application/json’
    request.setRawHeader("Accept", "application/json");
    qDebug() << Q_FUNC_INFO << request.url();
    netmanager->get(request);

}

void TP5_WeatherStation::weatherReplyFinished(QNetworkReply* reply)
{
    // managing response here (with errors handling if possible)

    if (reply->error() != QNetworkReply::NoError)
    {
        //Network Error
        qDebug() << reply->error() << "=>" << reply->errorString();
    }
    else if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200) { 
        
        QByteArray datas = reply->readAll();

        QJsonDocument jsonResponse = QJsonDocument::fromJson(datas);
        QJsonObject jsonObj = jsonResponse.object();

        QJsonObject mainObj = jsonObj["main"].toObject();
        double temp = mainObj["temp"].toDouble();
        qDebug() << temp;
        double temp_min = mainObj["temp_min"].toDouble();
        qDebug() << temp_min;
        double temp_max = mainObj["temp_max"].toDouble();
        qDebug() << temp_max;

        QJsonObject coordObj = jsonObj["coord"].toObject();
        double lon = coordObj["lon"].toDouble();
        qDebug() << lon;
        double lat = coordObj["lat"].toDouble();
        qDebug() << lat;

        QJsonArray weatherArray = jsonObj["weather"].toArray();
        QJsonObject object;

        for (auto w : weatherArray) {
            object = w.toObject();

            qDebug() << object.value("id").toInt();
            qDebug() << object.value("main").toString();
            qDebug() << object.value("description").toString(); 
            qDebug() << object.value("icon").toString();
        }

        weatherReport->setDatas(
            object.value("main").toString(), 
            object.value("description").toString(),
            temp,
            temp_min,
            temp_max,
            lon,
            lat
        );

    } else { 

        qDebug() << "Echec de connection à L'API";
    }

    //don't forget to delete when no more requested
    reply->deleteLater();
}

void TP5_WeatherStation::pollutionRequest() {

    // Pour tester les entêtes HTTP
    QString URL = "https://api.openweathermap.org/data/2.5/air_pollution/forecast?lat=46.0398&lon=5.4133&units=metric&lang=fr&appid=0f86cd77eba28eed506786b92de7bb40";
    QUrl url(URL);
    QNetworkRequest request;

    request.setUrl(url);
    //--header ’Accept: application/json’
    request.setRawHeader("Accept", "application/json");
    qDebug() << Q_FUNC_INFO << request.url();
    pollutionmanager->get(request);
}

void TP5_WeatherStation::pollutionReplyFinished(QNetworkReply* reply)
{
    // managing response here (with errors handling if possible)

    if (reply->error() != QNetworkReply::NoError)
    {
        //Network Error
        qDebug() << reply->error() << "=>" << reply->errorString();
    }
    else if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200) {

        QByteArray datas = reply->readAll();

        QJsonDocument jsonResponse = QJsonDocument::fromJson(datas);
        QJsonObject jsonObj = jsonResponse.object();

        QJsonArray weatherArray = jsonObj["list"].toArray();
        QJsonObject main;
        int dt;
        qDebug() << "Debut";
        for (auto w : weatherArray) {
            main = w.toObject().value("main").toObject();
            dt = w.toObject().value("dt").toDouble();

            //QDateTime localTime = QDateTime::fromSecsSinceEpoch(aqi); // s to local
            //qint64 msdt = localTime.toMSecsSinceEpoch(); // local to ms

            qDebug() << "-------------------------------------";
            qDebug() << main.value("aqi").toInt();
            qDebug() << dt;
            //qDebug() << localTime;
            //qDebug() << msdt;
            qDebug() << "-------------------------------------";
            dbmanager->addData(dt, main.value("aqi").toInt());
        }
        qDebug() << "Fin";


    }
    else {

        qDebug() << "Echec de connection à L'API";
    }

    //don't forget to delete when no more requested
    reply->deleteLater();
}