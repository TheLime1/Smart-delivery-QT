#ifndef RECLAMATION_H
#define RECLAMATION_H
#include <QString>
#include<QSqlQueryModel>
#include<QSqlQuery>
#include <QRadioButton>
#include <QDateEdit>
#include<QDate>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include<QString>
#include <QKeyEvent>
#include <QStandardItemModel>
#include <QMap>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>

class Reclamation
{
public:
    //constructeurs
    Reclamation();
    Reclamation(int,QDate,bool,bool,QString,QString);

    QList<int> recupererToutesLesReferences();
    QList<int> recupererToutesLesDates();

    //Getters
    int getref();
    QDate getdatee();
    bool getretour_justifie();
    bool getretour_nonjustifie();
    QString getavis();
    QString getrfid();

    //setters
    void setref(int);
    void setdatee(QDate);
    void setretour_justifie(bool);
    void setretour_nonjustifie(bool);
    void setavis(QString);
    void setrfid(QString);

    bool ajouter_rec();
    bool supprimer_rec(int);
    bool modifier_rec(int);
    QSqlQueryModel * afficher_rec();
    QSqlQueryModel * rechercher_rec(int ref);
    QSqlQueryModel * rechercher_rec_date(QDate datee);

    int calculer();
    int retour(bool);
    float statistique(bool retour_nonjustifie);


    QSqlQueryModel * tri_ref_c();
    QSqlQueryModel * tri_ref_dec();
    QSqlQueryModel * tri_dates_c();
    QSqlQueryModel * tri_dates_dec();

    void afficherHistorique();

private:
    int ref;
    QDate datee;
    bool retour_justifie;
    bool retour_nonjustifie;
    QString avis;
    QString rfid ;

};

#endif // RECLAMATION_H
