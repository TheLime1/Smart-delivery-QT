#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QChartView>
#include <QLabel>

#include "commande.h"
#include "chat.h"
#include "gas.h"
#include "arduino.h"
#include "monitordialog.h"

//! DHIA
#include "livreur.h"

//! MARIEM
#include "client.h"
/// dora
#include "reclamation.h"
namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setTableViewModel(QAbstractItemModel *model);
    Commande &getCtmp();
    QtCharts::QChart *createPieChart(QAbstractItemModel *model);
    QtCharts::QChart *createBarChart(QAbstractItemModel *model);

private slots:
    void on_add_button_3_clicked();

    void on_delete_button_2_clicked();

    void on_cherch_button_2_clicked();

    void on_tabWidget_tabBarClicked(int index);

    void on_sortButton_2_clicked();

    void on_exportPDFbutton_2_clicked();

    void on_refrechButton_clicked();

    void on_map_button_clicked();

    void on_admin_dummy_button_clicked();

    void on_admin_remove_button_clicked();

    void on_sortButton_ref_2_clicked();

    void on_sortButton_address_3_clicked();

    void on_live_search_22_textChanged(const QString &arg1);

    void on_live_search_ref_2_textChanged(const QString &arg1);

    void on_chat_pop_button_2_clicked();

    void on_stats_button_2_clicked();

    void on_clear_button_3_clicked();

    void on_gas_button_clicked();

    void onMonitorButtonClicked(); // custom button made by code

    void update_label(); // read data from arduino

    //! KHALED
    void on_pushButton_clicked();

    //! DHIA
    void on_pushButton_ajouter_livreur_clicked();

    void on_pushButton_livreur_clicked();

    void on_pushButton_modifier_livreur_clicked();

    void on_PDF_clicked();

    void on_pushButton_assigner_livreur_clicked();

    void on_comboBox_livreur_currentIndexChanged(const QString &arg1);

    void on_pushButton_rechercher_livreur_clicked();

    void on_pushButton_rechercher_livreur_2_clicked();

    void on_pushButton_rechercher_livreur_3_clicked();

    //! MARIEM
    void on_pb_ajouter_client_clicked();
    void on_pb_supprimer_client_clicked();
    void on_pb_chercher_client_clicked();
    void on_pb_enregister_client_clicked();

    void on_pb_trie_adresse_client_clicked();
    void on_pb_trie_nom_client_clicked();
    void on_pb_trie_date_client_clicked();

    void on_pb_chercher_nom_client_clicked();
    void on_pb_chercher_cin_client_clicked();
    void on_pb_chercher_adresse_client_clicked();

    void on_pb_statistique_genre_client_clicked();
    void on_pb_statistique_client_fidele_clicked();

    void on_PDF_2_clicked();

    void clients_plus_fideles();

    void on_pb_passer_commande_clicked();

    void on_pb_statistique_adresse_client_clicked();

    // void on_pushButton_valider_clicked();

    // void on_pushButton_scan_clicked();
    // dora
    void on_pushButton_ajouter_clicked();

    void on_pushButton_ajouter_2_clicked();

    void on_pushButton_modifier_clicked();

    void on_pushButton_rechercher_clicked();

    void on_pushButton_supprimer_clicked();

    void on_statistique_clicked();

    void on_pushButton_statistique_clicked();

    void on_pushButton_pdf_clicked();

    void on_tri_ref_c_clicked();

    void on_comboBox_ref_mod_activated(const QString &arg1);

    void on_tri_ref_dec_clicked();

    void on_tri_dates_c_clicked();

    void on_tri_dates_dec_clicked();

    void on_pushButton_historique_clicked();

    void on_qr_code_clicked();

    void on_ref_qr_activated(const QString &arg1);

    void on_pushButton_import_clicked();

    void on_pushButton_rechercher_date_clicked();

    void on_pushButton_2_clicked();

    // void on_pushButton_clicked();

    // void on_pushButton_valider_clicked();

    // void on_pushButton_scan_clicked();

    void on_groupBox_clicked();

    void on_pb_envoyer_client_clicked();

    void on_refrech_sup_d_clicked();

private:
    //! AYMEN
    Ui::MainWindow *ui;
    Commande Ctmp;
    chat ch;
    gas g;
    monitordialog md;

    bool sortState;
    bool sortStateRef;
    bool sortStateAddress;
    QWidget *currentWidget = nullptr;

    Arduino uno;
    QByteArray data;

    //! DHIA
    Livreur Ltmp;

    //! MARIEM
    Client c;
    // dora
    Reclamation rec, R;
    Arduino A;
};

#endif // MAINWINDOW_H
