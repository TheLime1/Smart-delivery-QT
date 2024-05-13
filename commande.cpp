#include "commande.h"
#include <QString>
#include <QSqlQuery>
#include <QtDebug>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <QFile>
#include <QSqlRecord>

Commande::Commande()
{
    ref = "";
    livreur = "";
    client = "";
    address = "";
    dd = 0;
    mm = 0;
    yyyy = 0;
    prix = 0;
    stock_id = "";
    status = "";
}

Commande::Commande(QString ref, QString livreur, QString client, QString address, int dd, int mm, int yyyy, float prix, QString stock_id, QString status)
{
    this->ref = ref;
    this->livreur = livreur;
    this->client = client;
    this->address = address;
    this->dd = dd;
    this->mm = mm;
    this->yyyy = yyyy;
    this->prix = prix;
    this->stock_id = stock_id;
    this->status = status;
}

QString Commande::getref()
{
    return ref;
}

QString Commande::getlivreur()
{
    return livreur;
}

QString Commande::getclient()
{
    return client;
}

QString Commande::getaddress()
{
    return address;
}

int Commande::getdd()
{
    return dd;
}

int Commande::getmm()
{
    return mm;
}

int Commande::getyy()
{
    return yyyy;
}

float Commande::getprix()
{
    return prix;
}

QString Commande::getstock_id()
{
    return stock_id;
}

QString Commande::getstatus()
{
    return status;
}

/////////////////////////////////

void Commande::setref(QString ref)
{
    this->ref = ref;
}

void Commande::setlivreur(QString livreur)
{
    this->livreur = livreur;
}

void Commande::setclient(QString client)
{
    this->client = client;
}

void Commande::setaddress(QString address)
{
    this->address = address;
}

void Commande::setdd(int dd)
{
    this->dd = dd;
}

void Commande::setmm(int mm)
{
    this->mm = mm;
}

void Commande::setyy(int yyyy)
{
    this->yyyy = yyyy;
}

void Commande::setprix(float prix)
{
    this->prix = prix;
}

void Commande::setstock_id(QString stock_id)
{
    this->stock_id = stock_id;
}

void Commande::setstatus(QString status)
{
    this->status = status;
}

bool Commande::ajouter()
{
    QSqlQuery query;

    QString d = QString::number(dd);
    QString m = QString::number(mm);
    QString y = QString::number(yyyy);
    QString price = QString::number(prix);

    query.prepare("INSERT INTO commande (reference, livreur, client, destination, jour, mois, anne, prix, stock_id, status) "
                  "VALUES (:reference, :livreur, :client, :destination, :jour, :mois, :anne, :prix, :stock_id, :status)");
    query.bindValue(":reference", ref);
    query.bindValue(":livreur", livreur);
    query.bindValue(":client", client);
    query.bindValue(":destination", address);
    query.bindValue(":jour", d);
    query.bindValue(":mois", m);
    query.bindValue(":anne", y);
    query.bindValue(":prix", price);
    query.bindValue(":stock_id", stock_id);
    query.bindValue(":status", status);

    return query.exec();
}

bool Commande::supprimer(QString ref)
{
    QSqlQuery query;

    query.prepare("DELETE FROM commande WHERE reference = :reference");
    query.bindValue(":reference", ref);

    return query.exec();
}

QSqlQueryModel *Commande::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery("SELECT * FROM commande");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Reference"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Livreur"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Client"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Destination"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Jour"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Mois"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Anne"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Prix"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Stock ID"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Status"));

    return model;
}

bool Commande::rechercher(QString ref)
{
    QSqlQuery query;
    query.prepare("SELECT reference FROM commande WHERE reference = :ref");
    query.bindValue(":ref", ref);
    if (query.exec() && query.next())
    {
        qDebug();
        return true; // found
    }
    else
    {
        qDebug();
        return false; // not found
    }
}

bool Commande::modifier(QString ref, QString livreur, QString client, QString address, int dd, int mm, int yyyy, float prix, QString stock_id, QString status)
{
    QSqlQuery query;

    QString d = QString::number(dd);
    QString m = QString::number(mm);
    QString y = QString::number(yyyy);
    QString price = QString::number(prix);

    query.prepare("UPDATE commande SET livreur = :livreur, client = :client, destination = :destination, jour = :jour, mois = :mois, anne = :anne, prix = :prix, stock_id = :stock_id, status = :status WHERE reference = :reference");

    query.bindValue(":reference", ref);
    query.bindValue(":livreur", livreur);
    query.bindValue(":client", client);
    query.bindValue(":destination", address);
    query.bindValue(":jour", d);
    query.bindValue(":mois", m);
    query.bindValue(":anne", y);
    query.bindValue(":prix", price);
    query.bindValue(":stock_id", stock_id);
    query.bindValue(":status", status);

    return query.exec();
}

bool Commande::deleteall()
{
    QSqlQuery query;

    query.prepare("DELETE FROM commande");

    return query.exec();
}

void Commande::add_dummy_data()
{
    QSqlQuery query;

    // Generate random REF, jour, mois, and anne
    QString ref = "REF" + QString::number(qrand() % 1000 + 1); // Random REF between REF1 and REF1000
    int jour = qrand() % 31 + 1;                               // Random jour between 1 and 31
    int mois = qrand() % 12 + 1;                               // Random mois between 1 and 12
    int anne = qrand() % (2023 - 1950 + 1) + 1950;             // Random anne between 1950 and 2023

    // Define the possible destinations
    QStringList destinations;
    destinations << "Ariana"
                 << "Béja"
                 << "Ben Arous"
                 << "Bizerte"
                 << "Gabès"
                 << "Gafsa"
                 << "Jendouba"
                 << "Kairouan"
                 << "Kasserine"
                 << "Kebili"
                 << "Kef"
                 << "Mahdia"
                 << "Manouba"
                 << "Medenine"
                 << "Monastir"
                 << "Nabeul"
                 << "Sfax"
                 << "Sidi Bouzid"
                 << "Siliana"
                 << "Sousse"
                 << "Tataouine"
                 << "Tozeur"
                 << "Tunis"
                 << "Zaghouan";

    // Generate a random destination
    QString destination = destinations[qrand() % destinations.size()];

    // Define the possible queries
    QStringList queries;
    queries << QString("INSERT INTO commande (reference, livreur, client, destination, jour, mois, anne, prix, stock_id, status) "
                       "VALUES ('%1', 'Livreur1', '12345678', '%5', %2, %3, %4, 100, 'stock1', 'not delivered')")
                   .arg(ref)
                   .arg(jour)
                   .arg(mois)
                   .arg(anne)
                   .arg(destination)
            << QString("INSERT INTO commande (reference, livreur, client, destination, jour, mois, anne, prix, stock_id, status) "
                       "VALUES ('%1', 'Livreur2', '87654321', '%5', %2, %3, %4, 200, 'stock2', 'not delivered')")
                   .arg(ref)
                   .arg(jour)
                   .arg(mois)
                   .arg(anne)
                   .arg(destination)
            << QString("INSERT INTO commande (reference, livreur, client, destination, jour, mois, anne, prix, stock_id, status) "
                       "VALUES ('%1', 'Livreur3', '12345678', '%5', %2, %3, %4, 300, 'stock1', 'delivered')")
                   .arg(ref)
                   .arg(jour)
                   .arg(mois)
                   .arg(anne)
                   .arg(destination)
            << QString("INSERT INTO commande (reference, livreur, client, destination, jour, mois, anne, prix, stock_id, status) "
                       "VALUES ('%1', 'Livreur1', '87654321', '%5', %2, %3, %4, 400, 'stock3', 'delivered')")
                   .arg(ref)
                   .arg(jour)
                   .arg(mois)
                   .arg(anne)
                   .arg(destination)
            << QString("INSERT INTO commande (reference, livreur, client, destination, jour, mois, anne, prix, stock_id, status) "
                       "VALUES ('%1', 'Livreur2', '12345678', '%5', %2, %3, %4, 500, 'stock2', 'not delivered')")
                   .arg(ref)
                   .arg(jour)
                   .arg(mois)
                   .arg(anne)
                   .arg(destination);

    // Generate a random index
    int randomIndex = qrand() % queries.size();

    // Execute the selected query
    query.exec(queries[randomIndex]);
}

void Commande::sql_csv()
{
    QSqlQuery query;
    if (query.exec("SELECT * FROM commande"))
    {
        QFile file("C:/Users/everp/Documents/GitHub/2a23-smart_delivery/chatbot/database_1L/dbraw.csv");
        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        {
            QTextStream data(&file);
            QStringList strList;

            // Write the column names to the CSV file
            QSqlRecord rec = query.record();
            for (int i = 0; i < rec.count(); i++)
            {
                strList << "\"" << rec.fieldName(i) << "\"";
            }
            data << strList.join(", ") << "\n";

            // Write the data rows to the CSV file
            while (query.next())
            {
                strList.clear();
                for (int i = 0; i < rec.count(); i++)
                {
                    strList << "\"" << query.value(i).toString() << "\"";
                }
                data << strList.join(", ") << "\n";
            }
            file.close();
        }
    }
    else
    {
        qDebug() << "Error: " << query.lastError().text();
    }
}
