#ifndef LIVREUR_H
#define LIVREUR_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Livreur
{
    QString reference, etat, colis;
    int autonomie, capacite, vitesse;
    QString reference_commande;

public:
    Livreur();
    Livreur(QString,int,int,int,QString);

    QString getReference(){return reference;}
    int getAutonomie(){return autonomie;}
    int getCapacite(){return capacite;}
    int getVitesse(){return vitesse;}
    QString getEtat(){return etat;};


    void setReference(QString r){reference=r;}
    void setAutonomie(int a){autonomie=a;}
    void setCapacite(int c){capacite=c;}
    void setVitesse(int v){vitesse=v;}
    void setEtat(QString e){etat=e;};


    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(QString);
    bool modifier(QString,int,int,int,QString);
    bool rechercher(QString ref);
    bool rechercherAutonomie(int aut);
    bool rechercherCapacite(int cap);
    bool Disponible();
    bool assignerLivraison(QString reference, QString referenceCommande);

    QString getReferenceCommande() const { return reference_commande; }
    void setReferenceCommande(const QString &ref) { reference_commande = ref; }
};

#endif // LIVREUR_H
