#ifndef PRODUIT_H
#define PRODUIT_H

#include <QSqlQuery>
#include <QSqlQueryModel>

class produit
{
public:
    produit();
    produit(QString,QString,QString,QString,QString,int,double);

    bool ajouter();
    bool modifier(QString);
    QSqlQueryModel * afficher();
    bool supprimer(QString);

    QString id,type,source,etat,status;
    double prix;
    int quantite;

};

#endif // PRODUIT_H
