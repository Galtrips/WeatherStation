#include "viewpollution.h"
#include <QDebug>
#include <QDateTime>

ViewPollution::ViewPollution(DbManager* dbm, QWidget* widget) :dbm(dbm), widget(widget)
{
    series = new QLineSeries();

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(series);
    chart->setTitle("Simple date/line chart example");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    //    chart->createDefaultAxes();
    axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("HH:mm");
    axisX->setTitleText("Date");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Air Quality Indice");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QGridLayout *chartLayout = new QGridLayout;
    chartLayout->addWidget(chartView, 0, 0);
    chartLayout->setContentsMargins(0, 0, 0, 0);

    // update chart
    widget->setLayout(chartLayout);

    update();
}

void ViewPollution::update()
{
    series->clear();

    // example on how to convert DateTime to Unix,UTC in milliseconds
    QDateTime currentdt = QDateTime::currentDateTime();
    qint64 msdt = currentdt.toMSecsSinceEpoch();
    qDebug() << currentdt << " " << QDateTime::fromMSecsSinceEpoch(msdt);

    // generate some mock values
    // get current time, and add 1 hour (3600000ms) to get next X entry
    // Warning, OpenWeatherMap provides dt in seconds
    // => so they will be registered in seconds into the DB...
    // => you will need to convert to milliseconds (*1000.) before to append
    int max = 0;
    int minq = 0;
    int maxq = 0;
    auto vec = dbm->getAllData();
    for (auto value : vec) {
        //qDebug() << value[0] << "==>" << value[1];

        QDateTime localTime = QDateTime::fromSecsSinceEpoch(value[0]); // s to local
        qint64 tempDt = localTime.toMSecsSinceEpoch(); //

        series->append(tempDt, value[1]);
        if (tempDt > max) {
            max = value[0];
        }
        if (value[1] > maxq) {
            maxq = value[1];
        }
        if (value[1] < minq) {
            minq = value[1];
        }
    }
    
    // alternative to append last points, just to show both ways
    /* *series << QPointF(msdt+3600000*5, 1) << QPointF(msdt+3600000*6, 3)
            << QPointF(msdt+3600000*7, 6) << QPointF(msdt+3600000*8, 3) << QPointF(msdt+3600000*9, 2);
    */
    // Compute min,max and update axis Ranges
    // Otherwise repaint() will no update the curve ;(
    QDateTime dt0;
    dt0.setMSecsSinceEpoch(msdt); //xmin for mock values
    QDateTime dt1;
    dt1.setMSecsSinceEpoch(msdt + 360000000); //xmax
    axisX->setRange( dt0,dt1);
    axisY->setRange(minq, maxq + 1); //ymin,ymax (do not change for these mock values)

    widget->repaint();
}
