#include "viewreport.h"
#include "dbmanager.h"

ViewReport::ViewReport(WeatherReport *wreport, Ui::TP5_WeatherStationClass *ui)
    : model(wreport), ui(ui)
{
    init();
    update();
}

void ViewReport::init()
{
    ui->lineEdit_temp->clear();
    ui->lineEdit_temp_min->clear();
    ui->lineEdit_temp_max->clear();
    ui->lineEdit_lon->clear();
    ui->lineEdit_lat->clear();
    ui->lineEdit_description->clear();
}

void ViewReport::update()
{
    ui->lineEdit_temp->setText(QString::number(model->getTemp()));
    ui->lineEdit_temp_min->setText(QString::number(model->getTempMin()));
    ui->lineEdit_temp_max->setText(QString::number(model->getTempMax()));
    ui->lineEdit_lon->setText(QString::number(model->getLon()));
    ui->lineEdit_lat->setText(QString::number(model->getLat()));
    ui->lineEdit_description->setText(model->getDescription());
}
