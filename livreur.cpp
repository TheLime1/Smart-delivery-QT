#include "livreur.h"
#include<QDebug>
#include<QObject>
#include<QMessageBox>
Livreur::Livreur(){
    reference=" ";
    autonomie=0;
    capacite=0;
    vitesse=0;
    etat=" ";
}


Livreur::Livreur(QString reference, int capacite, int autonomie, int vitesse, QString etat)
{
    this->reference = reference;
    this->capacite = capacite;
    this->autonomie = autonomie;
    this->vitesse = vitesse;
    this->etat = etat;
}

bool Livreur::ajouter()
{
    QSqlQuery query;
    QString a = QString::number(autonomie);
    QString c = QString::number(capacite);
    QString v = QString::number(vitesse);
    query.prepare("INSERT INTO LIVREUR (reference, capacite, autonomie, vitesse, etat) "
                  "VALUES (:reference, :capacite, :autonomie, :vitesse, :etat)");
    query.bindValue(":reference", reference);
    query.bindValue(":capacite", c);
    query.bindValue(":autonomie",a);
    query.bindValue(":vitesse", v);
    query.bindValue(":etat", etat);

    return query.exec();}

QSqlQueryModel *Livreur::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM livreur");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Reference"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Capacite"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Autonomie"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Vitesse"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Etat"));
    return model;
}



bool Livreur::supprimer(QString ref)
{
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM livreur WHERE reference = :reference");
    checkQuery.bindValue(":reference", ref);
    if (checkQuery.exec() && checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        QSqlQuery deleteQuery;
        deleteQuery.prepare("DELETE FROM livreur WHERE reference = :reference");
        deleteQuery.bindValue(":reference", ref);
        return deleteQuery.exec();
    } else {
        return false;
    }
}



bool Livreur::modifier(QString reference, int nouvelleAutonomie, int nouvelleCapacite, int nouvelleVitesse, QString nouvelleEtat) {
    QSqlQuery query;
    query.prepare("UPDATE livreur SET autonomie = :autonomie, capacite = :capacite, vitesse = :vitesse ,etat= :Etat WHERE reference = :reference");
    query.bindValue(":autonomie", nouvelleAutonomie);
    query.bindValue(":capacite", nouvelleCapacite);
    query.bindValue(":vitesse", nouvelleVitesse);
    query.bindValue(":Etat", nouvelleEtat);
    query.bindValue(":reference", reference);

    return query.exec() ;
}

bool Livreur::rechercher(QString ref)
{
    QSqlQuery query;
    query.prepare("SELECT reference FROM livreur WHERE reference = :ref");
    query.bindValue(":ref", ref);

    if (query.exec() && query.next())
    {
        qDebug() << "Livreur trouvé";
        return true;
    }
    else
    {
        qDebug() << "Livreur non trouvé";
        return false;
    }
}

bool Livreur::rechercherAutonomie(int aut)
{
    QSqlQuery query;
    query.prepare("SELECT reference FROM livreur WHERE autonomie = :aut");
    query.bindValue(":aut", aut);

    if (query.exec() && query.next())
    {
        qDebug() << "Livreur trouvé";
        return true;
    }
    else
    {
        qDebug() << "Livreur non trouvé";
        return false;
    }
}


bool Livreur::rechercherCapacite(int cap)
{
    QSqlQuery query;
    query.prepare("SELECT reference FROM livreur WHERE capacite = :cap");
    query.bindValue(":cap", cap);

    if (query.exec() && query.next())
    {
        qDebug() << "Livreur trouvé";
        return true;
    }
    else
    {
        qDebug() << "Livreur non trouvé";
        return false;
    }
}

bool Livreur::Disponible()
{
    QString etat = getEtat();
    return (etat == "disponible" || etat == "dispo");
}


bool Livreur::assignerLivraison(QString referenceLivreur, QString referenceCommande)
{
    if (!Disponible()) {
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE livreur SET etat = 'indisponible' WHERE reference = :referenceLivreur");
    query.prepare("SELECT reference FROM commande WHERE reference = :referenceCommande");
    query.bindValue(":referenceLivreur", referenceLivreur);
    query.bindValue(":referenceCommande", referenceCommande);

    return query.exec();
}



