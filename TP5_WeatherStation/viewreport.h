#ifndef VIEWREPORT_H
#define VIEWREPORT_H

#include <QGroupBox>

#include "weatherreport.h"
#include "observer.h"
#include "ui_TP5_WeatherStation.h"

/**
 * @brief The ViewReport class
 * @author FJa
 */
class ViewReport : public QObject, public Observer
{
    Q_OBJECT
private:
    WeatherReport *model;   //> model containing temp, localisation
    Ui::TP5_WeatherStationClass* ui;
public:
    ViewReport(WeatherReport *wreport, Ui::TP5_WeatherStationClass* ui);

    void init();
    void update() override;

};

#endif // VIEWREPORT_H
