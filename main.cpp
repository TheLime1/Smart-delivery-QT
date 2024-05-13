#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"
//dora
#include "mainwindow.h"
#include "mainslice.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QStatusBar>
#include <QtCharts/QChartView>
#include "donutbreakdownchart.h"
#include <QTimer>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include<QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Connection c;
    bool test = c.createconnect();
    DonutBreakdownChart *donutBreakdown = nullptr;

    if (test)
    {
        w.show();
        QMessageBox::critical(nullptr, QObject::tr("database is open"),
                              QObject::tr("connection successful.\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);
    }
    else
        QMessageBox::critical(nullptr, QObject::tr("database is not open"),
                              QObject::tr("connection failed.\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);
    donutBreakdown = new DonutBreakdownChart();
    donutBreakdown->setAnimationOptions(QChart::AllAnimations);
    donutBreakdown->setTitle("STATISTIQUE DES RETOURS");
    donutBreakdown->legend()->setAlignment(Qt::AlignRight);

    QMainWindow window;
    QChartView *chartView = new QChartView(donutBreakdown);
    chartView->setRenderHint(QPainter::Antialiasing);
    window.setCentralWidget(chartView);
    window.resize(800, 500);
    window.show();

    auto updateChart = [&]() {
        qDebug() << "Updating chart...";

        donutBreakdown->clearAllBreakdownSeries();

        QPieSeries *justifieSeries = new QPieSeries();
        justifieSeries->setName("RETOUR_JUSTIFIE");

        QPieSeries *nonJustifieSeries = new QPieSeries();
        nonJustifieSeries->setName("RETOUR_NONJUSTIFIE");


        QString queryStr = "SELECT RETOUR_JUSTIFIE, RETOUR_NONJUSTIFIE FROM reclamation";

        QSqlQuery query;
        if (query.exec(queryStr)) {
            while (query.next()) {
                int retourJustifieCount = query.value(0).toInt();
                int retourNonJustifieCount = query.value(1).toInt();

                qDebug() << "Retour justifié count:" << retourJustifieCount;
                qDebug() << "Retour non justifié count:" << retourNonJustifieCount;

                justifieSeries->append("RETOUR_JUSTIFIE", retourJustifieCount);
                nonJustifieSeries->append("RETOUR_NONJUSTIFIE", retourNonJustifieCount);
            }

            donutBreakdown->addJustifieSeries(justifieSeries, Qt::red);
            donutBreakdown->addNonJustifieSeries(nonJustifieSeries, Qt::blue);
        } else {
            qDebug() << "Erreur SQL : " << query.lastError().text();
        }
        qDebug() << "Chart updated.";
        donutBreakdown->updateChartTitle();
    };

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, updateChart);
    timer.start(3000);  // Définir la durée souhaitée en millisecondes
    return a.exec();
}
