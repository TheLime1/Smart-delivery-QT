#ifndef COMMANDE_H
#define COMMANDE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Commande
{
public:
    Commande();
    Commande(QString, QString, QString, QString, int, int, int, float, QString, QString);
    QString getref();
    QString getaddress();
    QString getlivreur();
    QString getclient();
    int getdd();
    int getmm();
    int getyy();
    float getprix();
    QString getstock_id();
    QString getstatus();

    void setref(QString);
    void setaddress(QString);
    void setlivreur(QString);
    void setclient(QString);
    void setdd(int);
    void setmm(int);
    void setyy(int);
    void setprix(float);
    void setstock_id(QString);
    void setstatus(QString);

    bool ajouter();
    QSqlQueryModel *afficher();
    bool supprimer(QString);
    bool rechercher(QString);
    bool modifier(QString, QString, QString, QString, int, int, int, float, QString, QString);
    bool deleteall();
    void add_dummy_data();

    void sql_csv();

private:
    QString ref;
    QString livreur;
    QString client;
    QString address;
    int dd, mm, yyyy;
    float prix;
    QString stock_id;
    QString status;
};

#endif // COMMANDE_H
