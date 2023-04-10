#ifndef VIEWPOLLUTION_H
#define VIEWPOLLUTION_H

#include <QWidget>
#include <QValueAxis>
#include <QDateTimeAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QChartGlobal>
#include <QGridLayout>
#include "dbmanager.h"
#include "observer.h"

#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
QT_CHARTS_USE_NAMESPACE
#endif

//using namespace QtCharts;
QT_USE_NAMESPACE

/**
 * @brief The ViewPollution class
 * @author Fja
 */
class ViewPollution : public QObject, public Observer
{
    Q_OBJECT
private:
    DbManager *dbm; //> DB, where to get the data (dt,aqi)
    QWidget* widget;
    QLineSeries *series; //> contains data to be displayed
    QDateTimeAxis *axisX; //> need to be updated on data change
    QValueAxis* axisY; //> need to be updated on data change
public:
    ViewPollution(DbManager*, QWidget*);

    void update() override;
};

#endif // VIEWPOLLUTION_H
