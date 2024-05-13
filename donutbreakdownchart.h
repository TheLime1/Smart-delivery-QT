#ifndef DONUTBREAKDOWNCHART_H
#define DONUTBREAKDOWNCHART_H

#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>

QT_CHARTS_USE_NAMESPACE

class DonutBreakdownChart : public QChart
{
public:
    void addJustifieSeries(QPieSeries *series, QColor color);
    void addNonJustifieSeries(QPieSeries *series, QColor color);

    DonutBreakdownChart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
    void addBreakdownSeries(QPieSeries *series, QColor color);
    void clearAllBreakdownSeries();
    void updateChartTitle();
private:
    void recalculateAngles();
    void updateLegendMarkers();

private:
    QPieSeries *m_mainSeries;
};

#endif // DONUTBREAKDOWNCHART_H
