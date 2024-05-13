#ifndef GESTION_PRODUIT_H
#define GESTION_PRODUIT_H

#include <QMainWindow>
#include <QSortFilterProxyModel>
#include <QTextTableFormat>
#include <QStandardItemModel>
#include <QFileDialog>
#include <QFile>
#include "connection.h"
#include "mailing.h"
#include "produit.h"
#include "stat_combo.h"
#include "exportexcelobject.h"
#include "arduino.h"

namespace Ui {
class gestion_produit;
}

class gestion_produit : public QMainWindow
{
    Q_OBJECT

public:
    explicit gestion_produit(QWidget *parent = nullptr);
    ~gestion_produit();

    produit tmp;
    QSortFilterProxyModel *proxy;

    QString get_id () const ;
    QString get_type () const ;
    QString get_source() const;
    QString get_etat() const ;
    QString get_status() const ;
    double get_prix() const;
    int get_quantite() const;

    bool verif_dechet();

    void fill_form(QString);
    void clear_form();
    void show_tables();

    void sendMail();

private slots:


    void on_ajouter_clicked();

    void on_modifier_clicked();

    void on_supprimer_clicked();

    void on_exporter_clicked();

    void on_stat_clicked();

    void on_tableau_produit_clicked(const QModelIndex &index);

    void on_tableau_produit_doubleClicked(const QModelIndex &index);

    void on_sel_col_currentIndexChanged(int index);

    void on_rech_textChanged(const QString &arg1);



    void on_pave_clicked();



    void on_pave_2_clicked();

private:
    Ui::gestion_produit *ui;
    QString selected="";
    int sel_col=-1;
    stat_combo *s;
    QStringList files;
    //Connexion c;
    QString mail_sender="chihab.khaled@esprit.tn", //matbadaltch
            mail_password="211JMT9786", // matbadalch
            mail_destination="chihab.khaled@esprit.tn", //lechkoun bch tab3th
            mail_subject="Status Changed", // titre mail
            mail_text="the status of this item is now STATUS1"; // contenu ta3 mail
    /*QString mail_sender="khaled.chiheb200@gmail.com", //matbadaltch
                mail_password="sznc taqr oqzc lpjk", // matbadalch
                mail_destination="chihab.khaled@esprit.tn", //lechkoun bch tab3th
                mail_subject="Status Changed", // titre mail
                mail_text="the status of this item is now STATUS1"; // contenu ta3 mail*/
    Mailing mailer;
    Arduino A;

};

#endif // GESTION_PRODUIT_H
