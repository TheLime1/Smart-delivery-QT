#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QCloseEvent>
#include <QTimer>

#include "monitordialog.h"
#include "ui_monitordialog.h"
#include "arduino.h"

monitordialog::monitordialog(const QString &id, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::monitordialog),
      id(id),
      firstCall(true)
{
    ui->setupUi(this);
}

void monitordialog::setId(const QString &id)
{
    this->id = id;

    int ret = uno.connect_arduino();
    switch (ret)
    {
    case 0:
        qDebug() << "arduino is available and connected to : " << uno.getarduino_port_name();
        break;
    case 1:
        qDebug() << "arduino is available but not connected to : " << uno.getarduino_port_name();
        break;
    case -1:
        qDebug() << "arduino is not available";
        break;
    }

    // Update the labels here, just like in the constructor
    QSqlQuery query1, query2;
    query1.prepare("SELECT STOCK_ID FROM COMMANDE WHERE REFERENCE = :id");
    query1.bindValue(":id", id);
    if (query1.exec() && query1.next())
    {
        QString stock_id = query1.value(0).toString();
        query2.prepare("SELECT TEMP_BEST FROM PRODUIT WHERE ID_STOCK = :stock_id");
        query2.bindValue(":stock_id", stock_id);
        if (query2.exec() && query2.next())
        {
            idealTemp = query2.value(0).toFloat();
            ui->ideal_t_label->setText("ideal temp: " + QString::number(idealTemp) + " C");

            ui->hc_status->setText("Waiting for Arduino data...");
            ui->hc_status->setStyleSheet("QLabel { color : black; font: bold 30px Arial, sans-serif; }");

            // send the ideal temp to the arduino
            qDebug() << "Sending ideal temp to Arduino: " << idealTemp;
            std::string temp_str = QString::number(idealTemp).toStdString();
            uno.write_to_arduino(QByteArray::fromStdString(temp_str));
        }
        else
        {
            qDebug() << "Failed to execute query2: " << query2.lastError();
        }
    }
    else
    {
        qDebug() << "Failed to execute query1: " << query1.lastError();
    }
    QObject::connect(uno.getserial(), SIGNAL(readyRead()), this, SLOT(update_label_m()));
}

void monitordialog::update_label_m()
{
    static QString buffer;        // buffer to hold incoming data
    static bool firstCall = true; // flag to check if it's the first call

    buffer += uno.read_from_arduino(); // append new data to buffer

    int newlineIndex = buffer.indexOf('\n'); // find newline character

    // if newline character found, we have a complete line
    if (newlineIndex != -1)
    {
        QString data = buffer.left(newlineIndex); // get the complete line
        buffer.remove(0, newlineIndex + 1);       // remove the line from the buffer

        qDebug() << "data : " << data;
        ui->realTempLabel->setText("real temp : " + data); // Update idle_temp label

        QStringList parts = data.split(' ');
        if (parts.size() > 0)
        {
            bool ok;
            float realTemp = parts[0].toFloat(&ok);
            if (ok)
            {
                if (firstCall)
                {
                    QTimer::singleShot(3000, [this, realTemp]() { // wait for 3 seconds
                        firstCall = false;
                        updateHcStatus(realTemp);
                    });
                }
                else
                {
                    updateHcStatus(realTemp);
                }
            }
            else
            {
                qDebug() << "Failed to convert data to float: " << data;
            }
        }
    }
}

void monitordialog::updateHcStatus(float realTemp)
{
    if (realTemp < idealTemp)
    {
        ui->hc_status->setText("Heating...");
        ui->hc_status->setStyleSheet("QLabel { color : red; font: bold 30px Arial, sans-serif; }");
    }
    else if (realTemp > idealTemp)
    {
        ui->hc_status->setText("Cooling...");
        ui->hc_status->setStyleSheet("QLabel { color : blue; font: bold 30px Arial, sans-serif; }");
    }
    else
    {
        ui->hc_status->setText("Waiting for Arduino data...");
        ui->hc_status->setStyleSheet("QLabel { color : black; font: bold 30px Arial, sans-serif; }");
    }
}

monitordialog::~monitordialog()
{
    delete ui;
    // uno.write_to_arduino("idle");
}

void monitordialog::on_close_monitor_clicked()
{
    // uno.write_to_arduino("idle\n");
    this->close();
}

void monitordialog::closeEvent(QCloseEvent *event)
{
    uno.write_to_arduino("idle\n");
    firstCall = true;
    event->accept();
    QDialog::closeEvent(event);
}
