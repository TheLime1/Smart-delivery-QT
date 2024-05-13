#include "client.h"
#include "connection.h"
#include <QSqlQuery>
#include <QtDebug>
#include <QObject>
Client::Client()
{
cin=0; nom=""; prenom=""; adresse="";  numero=0; mail="";  genre="";  date_naissance=""; pointsFidelite = 0;
}

Client::Client(int cin,QString nom,QString prenom,QString adresse,int numero,QString mail,QString genre,QString date_naissance)
{
    this->cin=cin;
    this->prenom=prenom;
    this->nom=nom;
    this->adresse=adresse;
    this->numero=numero;
    this->mail=mail;
    this->genre=genre;
    this->date_naissance=date_naissance;
    this->pointsFidelite = 0;
}
int Client::getcin(){return cin;}
QString Client::getnom(){return nom;}
QString Client::getprenom(){return prenom;}
QString Client::getadresse(){return adresse;}
int Client::getnumero(){return numero;}
QString Client::getmail(){return mail;}
QString Client::getgenre(){return genre;}
QString Client::getdate_naissance(){return date_naissance;}

void Client::setcin(int cin){this->cin=cin;}
void Client::setnom(QString nom){this->nom=nom;}
void Client::setnumero(int numero){this->numero=numero;}
void Client::setprenom(QString prenom){this->prenom=prenom;}
void Client::setadresse(QString adresse){this->adresse=adresse;}
void Client::setmail(QString mail){this->mail=mail;}
void Client::setgenre(QString genre){this->genre=genre;}
void Client::setdate_naissance(QString date_naissance){this->date_naissance=date_naissance;}

void Client::incrementerPointsFidelite()
{
    pointsFidelite += 10;
}

int Client::getPointsFidelite() const
{
    return pointsFidelite;
}


bool Client::ajouter()
{
    QSqlQuery query;
    QString cin_string=QString::number(cin);
          query.prepare("INSERT INTO CLIENT (ADRESSE,CIN, DATE_NAISSANCE , GENRE , MAIL , NOM, NUMERO, PRENOM , POINTSFIDELITE) "
                        "VALUES (:ADRESSE, :CIN, :DATE_NAISSANCE , :GENRE , :MAIL , :NOM,  :NUMERO , :PRENOM ,:POINTSFIDELITE)");
          query.bindValue(":ADRESSE", adresse);
          query.bindValue(":CIN", cin_string);
          query.bindValue(":DATE_NAISSANCE", date_naissance);
          query.bindValue(":GENRE", genre);
          query.bindValue(":MAIL", mail);
          query.bindValue(":NOM", nom);
          query.bindValue(":NUMERO", numero);
          query.bindValue(":PRENOM", prenom);
          query.bindValue(":POINTSFIDELITE", pointsFidelite);
          qDebug() << query.lastError().text();
         return query.exec();
}
bool Client::supprimer(int cin)
{
    QSqlQuery query;
    QString cin_string=QString::number(cin);
          query.prepare("Delete from client where CIN=:CIN");
          query.bindValue(":CIN", cin);

         return query.exec();

}
QSqlQueryModel*  Client::afficher()
{
    QSqlQueryModel* model=new QSqlQueryModel();
         model->setQuery("SELECT* FROM CLIENT");
         model->setHeaderData(0, Qt::Horizontal, QObject::tr("cin"));
         model->setHeaderData(1, Qt::Horizontal, QObject::tr("date de naissance"));
         model->setHeaderData(2, Qt::Horizontal, QObject::tr("adresse"));
         model->setHeaderData(3, Qt::Horizontal, QObject::tr("genre"));
         model->setHeaderData(4, Qt::Horizontal, QObject::tr("mail"));
         model->setHeaderData(5, Qt::Horizontal, QObject::tr("nom"));
         model->setHeaderData(6, Qt::Horizontal, QObject::tr("numero"));
         model->setHeaderData(7, Qt::Horizontal, QObject::tr("prenom"));
         model->setHeaderData(8, Qt::Horizontal, QObject::tr("pointsFidelite"));
    return model;
}
bool Client::existe(int cin)
{
    QSqlQuery query;
    query.prepare("SELECT cin FROM CLIENT WHERE cin = :cin");
    query.bindValue(":cin", cin);
    if (query.exec() && query.next())
    {
        qDebug();
        return true;
    }else
    {
        qDebug();
         return false;
    }
}
bool Client::modifier(QString adresse, int cin, QString date_naissance, QString genre, QString mail, QString nom, int numero, QString prenom)
{
    QSqlQuery query;
    query.prepare("UPDATE CLIENT SET ADRESSE = :ADRESSE, DATE_NAISSANCE = :DATE_NAISSANCE, GENRE = :GENRE, MAIL = :MAIL, NOM = :NOM, NUMERO = :NUMERO, PRENOM = :PRENOM WHERE CIN = :CIN");
    query.bindValue(":ADRESSE", adresse);
    query.bindValue(":CIN", cin);
    query.bindValue(":DATE_NAISSANCE", date_naissance);
    query.bindValue(":GENRE", genre);
    query.bindValue(":MAIL", mail);
    query.bindValue(":NOM", nom);
    query.bindValue(":NUMERO", numero);
    query.bindValue(":PRENOM", prenom);

    return query.exec();
}
QSqlQueryModel* Client::trierParAdresse()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM CLIENT ORDER BY ADRESSE ASC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("cin"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("adresse"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("numero"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("mail"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("genre"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("date de naissance"));
    return model;
}
QSqlQueryModel* Client::trierParNom()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM CLIENT ORDER BY NOM ASC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("cin"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("adresse"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("numero"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("mail"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("genre"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("date de naissance"));
    return model;
}
QSqlQueryModel* Client::trierParDateNaissance()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM CLIENT ORDER BY DATE_NAISSANCE ASC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("cin"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("adresse"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("numero"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("mail"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("genre"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("date de naissance"));
    return model;
}


QSqlQueryModel* Client::rechercherParNom(const QString &nom)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryStr = "SELECT * FROM CLIENT WHERE NOM LIKE :nom";
    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(":nom", "%" + nom + "%");
    query.exec();
    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("cin"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("adresse"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("numero"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("mail"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("genre"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("date de naissance"));
    return model;
}
QSqlQueryModel* Client::rechercherParAdresse(const QString &adresse)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryStr = "SELECT * FROM CLIENT WHERE ADRESSE LIKE :adresse";
    QSqlQuery query;
    query.prepare(queryStr);
    query.bindValue(":adresse", "%" + adresse + "%");
    query.exec();
    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("cin"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("adresse"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("numero"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("mail"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("genre"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("date de naissance"));
    return model;
}
QSqlQueryModel* Client::rechercherParCIN(int cin)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT * FROM CLIENT WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    if (query.exec())
    {
        model->setQuery(query);
        return model;
    }
    else
    {
        qDebug() << "Erreur lors de la recherche par CIN : " << query.lastError().text();
        return nullptr;
    }
}
