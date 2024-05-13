#ifndef CLIENT_H
#define CLIENT_H

#include<QString>
#include<QSqlQuery>
#include<QSqlQueryModel>
class Client
{
public:
    Client();
    Client(int,QString,QString,QString,int,QString,QString,QString );
    int getcin();
    QString getnom();
    QString getprenom();
    QString getadresse();
    int getnumero();
    QString getmail();
    QString getgenre();
    QString getdate_naissance();

    void setcin(int);
    void setnom(QString);
    void setprenom(QString);
    void setadresse(QString);
    void setnumero(int);
    void setmail(QString);
    void setgenre(QString);
    void setdate_naissance(QString);

    void incrementerPointsFidelite();
    int getPointsFidelite() const;

    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(int) ;
    bool existe(int);
    bool modifier(QString,int ,QString,QString,QString,QString, int ,QString);

    QSqlQueryModel* trierParAdresse();
    QSqlQueryModel* trierParNom();
    QSqlQueryModel* trierParDateNaissance();

    QSqlQueryModel* rechercherParNom(const QString &nom);
    QSqlQueryModel* rechercherParAdresse(const QString &adresse);
    QSqlQueryModel* rechercherParCIN(int cin);


private:
    int cin,numero, pointsFidelite;
    QString nom,prenom,adresse,mail,genre,date_naissance;
};

#endif // CLIENT_H
