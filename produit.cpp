#include "produit.h"

produit::produit()
{
}

produit::produit(QString s1, QString s2, QString s3, QString s4, QString s5, int x, double y)
{

    id = s1;
    type = s2;
    source = s3;
    etat = s4;
    status = s5;
    quantite = x;
    prix = y;
}

bool produit::ajouter()
{

    QSqlQuery query;

    query.prepare("INSERT INTO produit (id,type,source,etat,status,quantite,prix) VALUES (:id_stock,:type,:source,:etat,:status,:quantite,:prix)");

    query.bindValue(":id_stock", id);
    query.bindValue(":type", type);
    query.bindValue(":source", source);
    query.bindValue(":etat", etat);
    query.bindValue(":status", status);
    query.bindValue(":quantite", quantite);
    query.bindValue(":prix", prix);

    return query.exec();
}

bool produit::modifier(QString selected)
{

    QSqlQuery query;

    query.prepare(" UPDATE produit SET type=:type, source=:source, etat=:etat, status=:status, quantite=:quantite, prix=:prix "
                  " where id= :id_stock");

    query.bindValue(":id_stock", selected);
    query.bindValue(":type", type);
    query.bindValue(":source", source);
    query.bindValue(":etat", etat);
    query.bindValue(":status", status);
    query.bindValue(":quantite", quantite);
    query.bindValue(":prix", prix);

    return query.exec();
}

QSqlQueryModel *produit::afficher()
{

    QSqlQueryModel *modal = new QSqlQueryModel();
    modal->setQuery("SELECT * FROM produit");

    return modal;
}
bool produit::supprimer(QString selected)
{

    QSqlQuery query;
    query.prepare("Delete from produit where id = :id_stock ");

    query.bindValue(":id_stock", selected);
    return query.exec();
}
