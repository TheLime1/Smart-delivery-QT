#include <QDialog>
#include <QString>
#include <QMessageBox>
#include <QSqlQuery>

#include "dialog.h"
#include "ui_dialog.h"
#include "commande.h"
#include "mainwindow.h"

Dialog::Dialog(MainWindow *mainWindow, QWidget *parent) : QDialog(parent),
                                                          ui(new Ui::Dialog),
                                                          mainWindow(mainWindow)
{
    ui->setupUi(this);
    ui->m_day_input->setValidator(new QIntValidator(1, 31, this));
    ui->m_month_input->setValidator(new QIntValidator(1, 12, this));
    ui->m_year_input->setValidator(new QIntValidator(0, 9999, this));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::afficherref(QString ref)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM commande WHERE reference = :ref");
    query.bindValue(":ref", ref);

    if (query.exec())
    {
        while (query.next())
        {
            ui->m_ref_input->setText(query.value("reference").toString());
            ui->m_address_input->setText(query.value("destination").toString());
            ui->m_day_input->setText(query.value("jour").toString());
            ui->m_month_input->setText(query.value("mois").toString());
            ui->m_year_input->setText(query.value("anne").toString());
            ui->m_prix_input->setText(query.value("prix").toString());

            // Populate the client combo box
            QSqlQuery queryClient;
            queryClient.exec("SELECT CIN_C FROM CLIENT");
            while (queryClient.next())
            {
                QString clientId = queryClient.value(0).toString();
                ui->m_client_combo->addItem(clientId);
            }

            // Populate the livreur combo box
            QSqlQuery queryLivreur;
            queryLivreur.exec("SELECT ID_LIV FROM LIVREUR");
            while (queryLivreur.next())
            {
                QString livreurId = queryLivreur.value(0).toString();
                ui->m_livreur_combo->addItem(livreurId);
            }

            // Populate the stock combo box
            QSqlQuery queryStock;
            queryStock.exec("SELECT ID_STOCK FROM STOCK");
            while (queryStock.next())
            {
                QString stockId = queryStock.value(0).toString();
                ui->m_stock_combo->addItem(stockId);
            }
            QString currentStatus = query.value("status").toString();
            ui->m_status_combo->clear();
            if (currentStatus == "delivered")
            {
                ui->m_status_combo->addItem("delivered");
                ui->m_status_combo->addItem("not delivered");
            }
            else if (currentStatus == "not delivered")
            {
                ui->m_status_combo->addItem("not delivered");
                ui->m_status_combo->addItem("delivered");
            }

            // Set the current index of the combo boxes
            ui->m_client_combo->setCurrentText(query.value("client").toString());
            ui->m_livreur_combo->setCurrentText(query.value("livreur").toString());
            ui->m_stock_combo->setCurrentText(query.value("stock_id").toString());
            ui->m_status_combo->setCurrentText(currentStatus);
        }
    }
}

void Dialog::on_m_button_clicked()
{
    QString ref = ui->m_ref_input->text();

    QString livreur = ui->m_livreur_combo->currentText();
    QString client = ui->m_client_combo->currentText();
    QString adr = ui->m_address_input->text();
    int dd = ui->m_day_input->text().toInt();
    int mm = ui->m_month_input->text().toInt();
    int yyyy = ui->m_year_input->text().toInt();
    float prix = ui->m_prix_input->text().toFloat();
    QString stock_id = ui->m_stock_combo->currentText();
    QString status = ui->m_status_combo->currentText();

    Commande C(ref, livreur, client, adr, dd, mm, yyyy, prix, stock_id, status);

    bool test = C.modifier(ref, livreur, client, adr, dd, mm, yyyy, prix, stock_id, status);

    if (test)
    {
        // Refresh the table view in the main window
        mainWindow->setTableViewModel(mainWindow->getCtmp().afficher());

        // Display a message indicating success
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Commande modifiée.\n"
                                             "Click Cancel to exit."),
                                 QMessageBox::Cancel);

        // Close the dialog
        accept();
    }
    else
    {
        // Display an error message
        QMessageBox::critical(nullptr, QObject::tr("NOT OK"),
                              QObject::tr("Commande non modifiée.\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);
    }
}
