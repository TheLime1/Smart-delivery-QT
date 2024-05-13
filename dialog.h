#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QString>
#include "commande.h"

class MainWindow;
namespace Ui
{
    class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(MainWindow *mainWindow, QWidget *parent = nullptr);
    ~Dialog();
    void afficherref(QString ref);

private slots:
    void on_m_button_clicked();

private:
    Ui::Dialog *ui;
    MainWindow *mainWindow;
};

#endif // DIALOG_H
