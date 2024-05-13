#ifndef MONITORDIALOG_H
#define MONITORDIALOG_H

#include <QDialog>
#include <QCloseEvent>

#include "arduino.h"

namespace Ui
{
    class monitordialog;
}

class monitordialog : public QDialog
{
    Q_OBJECT

public:
    explicit monitordialog(const QString &id, QWidget *parent = nullptr);
    ~monitordialog();

    Ui::monitordialog *ui;
    float idealTemp;

    void setId(const QString &id);
    void updateHcStatus(float realTemp);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_close_monitor_clicked();
    void update_label_m();

private:
 Arduino uno;
    QString id;
    bool firstCall;
};

#endif // MONITORDIALOG_H
