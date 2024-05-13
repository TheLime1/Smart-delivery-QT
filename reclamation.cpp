#include "reclamation.h"
#include <QSqlQuery>
#include <QtDebug>
#include <QObject>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include<QDateEdit>
#include <QSqlRecord>
#include <QFile>
#include<QTextEdit>
#include <QVBoxLayout>
#include <iostream>
#include <QStandardItemModel>
#include <QtPrintSupport/QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QtSql>
#include "mainwindow.h"
#include "ui_mainwindow.h"



Reclamation::Reclamation()
{
ref=0;  datee=QDate::currentDate();      retour_justifie=false;     retour_nonjustifie=false;    avis="";   rfid="";
}

QList<int> Reclamation::recupererToutesLesReferences()
{
    QList<int> references;
    QSqlQuery query("SELECT ref FROM RECLAMATION");
    while (query.next())
    {
        int ref = query.value(0).toInt();
        references.append(ref);
    }
    return references;
}



Reclamation::Reclamation (int ref,QDate datee,bool retour_justifie,bool retour_nonjustifie,QString avis,QString rfid)
{this->ref=ref;  this->datee=datee;   this-> retour_justifie=retour_justifie;    this-> retour_nonjustifie=retour_nonjustifie;    this->avis=avis;   this->rfid=rfid;}

int Reclamation:: getref(){return ref;};

QDate Reclamation::getdatee(){return datee;};

bool Reclamation::getretour_justifie(){return retour_justifie;};

bool Reclamation::getretour_nonjustifie(){return retour_nonjustifie;};

QString Reclamation:: getavis(){return avis;};

QString Reclamation:: getrfid(){return rfid;};


void Reclamation::setref(int ref){this->ref=ref;}
void Reclamation::setdatee(QDate datee){this->datee=datee;}
void Reclamation::setretour_justifie(bool retour_justifie)
{this->retour_justifie=retour_justifie;}
void Reclamation::setretour_nonjustifie(bool retour_nonjustifie)
{this->retour_nonjustifie=retour_nonjustifie;}
void Reclamation::setavis(QString avis){this->avis=avis;}

void Reclamation::setrfid(QString rfid){this->rfid=rfid;}



bool Reclamation::ajouter_rec()
{
    QSqlQuery query;
    QString ref_string= QString::number(ref);

    query.prepare("INSERT INTO RECLAMATION (ref, datee, retour_justifie, retour_nonjustifie, avis,rfid) "
                  "VALUES (:ref, :datee, :retour_justifie, :retour_nonjustifie, :avis, :rfid)");
    query.bindValue(":ref", ref_string);
    query.bindValue(":datee", datee);
    query.bindValue(":retour_justifie", retour_justifie);
    query.bindValue(":retour_nonjustifie", retour_nonjustifie);
    query.bindValue(":avis", avis);
      query.bindValue(":rfid", rfid);

    // Exécution de la requête SQL
    bool queryResult = query.exec();

    // Enregistrement dans l'historique
    QFile file("C:/Users/everp/Documents/GitHub/2a23-smart_delivery/historique.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        // Gérer l'échec d'ouverture du fichier
        return false;
    }

    QTextStream cout(&file);
    QString d_info = QDateTime::currentDateTime().toString();
    QString message = d_info + " - Une réclamation a été ajoutée avec la reference " + QString::number(ref) + "\n";
    cout << message;

    file.close();

    return queryResult;
    //return query.exec();
}


bool Reclamation::supprimer_rec(int ref)
{
    QSqlQuery query;
    QString ref_string = QString::number(ref);
    query.prepare("Delete from RECLAMATION where REF=:REF");
    query.bindValue(":REF", ref_string);

    // Exécution de la requête SQL
    bool success = query.exec();

    // Enregistrement dans l'historique
    QFile file("C:/Users/everp/Documents/GitHub/2a23-smart_delivery/historique.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        // Gérer l'échec d'ouverture du fichier
        qDebug() << "Error opening history file for writing";
        return false;
    }

    QTextStream cout(&file);
    QString d_info = QDateTime::currentDateTime().toString();
    QString message = d_info + " - Réclamation avec la reference " + QString::number(ref) + " supprimée\n";
    cout << message;

    file.close();

    // Gestion des erreurs lors de la suppression
    if (!success) {
        qDebug() << "Error deleting record:" << query.lastError().text();
    }

    return success;
}

bool Reclamation::modifier_rec(int)
{
    QSqlQuery query;
    QString ref_string= QString::number(ref);
    query.prepare("UPDATE RECLAMATION SET DATEE=:DATEE, RETOUR_JUSTIFIE=:RETOUR_JUSTIFIE, RETOUR_NONJUSTIFIE=:RETOUR_NONJUSTIFIE, AVIS=:AVIS WHERE REF=:REF");
    query.bindValue(":DATEE", datee);
    query.bindValue("RETOUR_JUSTIFIE",retour_justifie);
    query.bindValue("RETOUR_NONJUSTIFIE", retour_nonjustifie);
    query.bindValue(":AVIS", avis);
    query.bindValue(":REF", ref_string);



    // Exécution de la requête SQL
    bool queryResult = query.exec();

    // Enregistrement dans l'historique
    QFile file("C:/Users/everp/Documents/GitHub/2a23-smart_delivery/historique.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        // Gérer l'échec d'ouverture du fichier
        qDebug() << "Error opening history file for writing";
        return false;
    }

    QTextStream cout(&file);
    QString d_info = QDateTime::currentDateTime().toString();
    QString message = d_info + " - Réclamation avec la reference " + QString::number(ref) + " mise à jour\n";
    cout << message;

    file.close();

    return queryResult;
}

QSqlQueryModel * Reclamation::afficher_rec()
{
QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT* FROM RECLAMATION");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("REF"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("DATE"));
    model->setHeaderData(2, Qt:: Horizontal, QObject::tr("RETOUR_JUSTIFIE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("RETOUR_NONJUSTIFIE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("AVIS"));


    // Enregistrement dans l'historique
    QFile file("C:/Users/everp/Documents/GitHub/2a23-smart_delivery/historique.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QTextStream cout(&file);
        QString d_info = QDateTime::currentDateTime().toString();
        QString message = d_info + " - Affichage des réclamations\n";
        cout << message;

        file.close();
    } else {
        qDebug() << "Error opening history file for writing";
    }

    return model;
}

QSqlQueryModel * Reclamation::rechercher_rec(int ref)
{
    QSqlQueryModel * model = new QSqlQueryModel();
    QSqlQuery query;

    query.prepare("SELECT * FROM RECLAMATION WHERE REF = :ref");
    query.bindValue(":ref", ref);


    if(query.exec())
    {
        model->setQuery(query);
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("REF"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("DATE"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("RETOUR_JUSTIFIE"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("RETOUR_NONJUSTIFIE"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("AVIS"));
    }

    else
        {
            qDebug() << "Erreur SQL lors de la recherche : " << query.lastError().text();
        }
    return model;
}

int Reclamation::calculer()
{
    QSqlQuery query;
        query.prepare("SELECT COUNT(*) FROM RECLAMATION");
        if (!query.exec())
        {
            qDebug() << "Erreur :" << query.lastError().text();
            return -1;
        }
        if (query.next())
        {
            int tot = query.value(0).toInt();
            qDebug() << "totale:" << tot;
            return tot;
        } else
        {
            qDebug() << "Aucun res.";
            return -1;
        }
}
int Reclamation::retour(bool retour_nonjustifie)
{
    QSqlQuery query;

        query.prepare("SELECT COUNT(*) FROM RECLAMATION WHERE retour_nonjustifie = :RETOUR_NONJUSTIFIE");
        query.bindValue(":RETOUR_NONJUSTIFIE", retour_nonjustifie);
        if (!query.exec()) {
            qDebug() << "Erreur :" << query.lastError().text();
            return -1;
        }
        if (query.next()) {
            int count = query.value(0).toInt();
            qDebug() << "Le nombre d'occurrences pour le retour non justifie" << retour_nonjustifie << "est de" << count;
            return count;
        }
        else
            {
                qDebug() << "Aucun resultat retourne par la requete.";
                return -1;
            }

}
float Reclamation::statistique(bool retour_nonjustifie)
{
    float stat=0;
    int tot;
    int count;
    tot=calculer();
    count=retour(retour_nonjustifie);
    qDebug()<<tot;
    qDebug()<<count;
if(tot!=-1)
{
    stat=static_cast<double>(count)/tot*100;
    qDebug() << "statistique de "<<retour_nonjustifie<<"="<<stat<<"%";
}
    return stat;
}

QSqlQueryModel * Reclamation::tri_ref_c()
{
    QSqlQueryModel * model=new QSqlQueryModel();

    model->setQuery("select * from reclamation  order by ref ASC");

    model->setHeaderData(0,Qt::Horizontal,QObject::tr("REF"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("DATEE"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("RETOUR_JUSTIFIE"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("RETOUR_NONJUSTIFIE"));
    model->setHeaderData(4,Qt::Horizontal,QObject::tr("AVIS"));


    return model;
}


QSqlQueryModel * Reclamation::tri_ref_dec()
{
    QSqlQueryModel * model=new QSqlQueryModel();

    model->setQuery("select * from reclamation  order by ref DESC");

    model->setHeaderData(0,Qt::Horizontal,QObject::tr("REF"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("DATEE"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("RETOUR_JUSTIFIE"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("RETOUR_NONJUSTIFIE"));
    model->setHeaderData(4,Qt::Horizontal,QObject::tr("AVIS"));

    return model;
}


QSqlQueryModel * Reclamation::tri_dates_c()
{
    QSqlQueryModel * model=new QSqlQueryModel();

    model->setQuery("select * from reclamation  order by datee ASC");

    model->setHeaderData(0,Qt::Horizontal,QObject::tr("REF"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("DATEE"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("RETOUR_JUSTIFIE"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("RETOUR_NONJUSTIFIE"));
    model->setHeaderData(4,Qt::Horizontal,QObject::tr("AVIS"));

    return model;
}

QSqlQueryModel * Reclamation::tri_dates_dec()
{
    QSqlQueryModel * model=new QSqlQueryModel();

    model->setQuery("select * from reclamation  order by datee DESC");

    model->setHeaderData(0,Qt::Horizontal,QObject::tr("REF"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("DATEE"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("RETOUR_JUSTIFIE"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("RETOUR_NONJUSTIFIE"));
    model->setHeaderData(4,Qt::Horizontal,QObject::tr("AVIS"));

    return model;
}


void Reclamation::afficherHistorique()
{
    // Charger le contenu du fichier d'historique
    QFile historyFile("C:/Users/everp/Documents/GitHub/2a23-smart_delivery/historique.txt");
    if (!historyFile.exists()) {
        if (historyFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            historyFile.close();
        } else {
            QMessageBox::warning(nullptr, "Erreur", "Impossible de créer le fichier d'historique.");
            return;
        }
    }

    if (historyFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&historyFile);
        QString historiqueContenu = in.readAll();
        historyFile.close();

        QDialog* historiqueDialog = new QDialog(nullptr);
        historiqueDialog->setWindowTitle("Historique");
        historiqueDialog->setMinimumSize(400, 300);

        QTextEdit* textEdit = new QTextEdit(historiqueDialog);
        textEdit->setPlainText(historiqueContenu);
        textEdit->setReadOnly(true);

        QVBoxLayout* layout = new QVBoxLayout(historiqueDialog);
        layout->addWidget(textEdit);

        historiqueDialog->exec();
    } else {
        QMessageBox::warning(nullptr, "Erreur", "Impossible d'ouvrir le fichier d'historique pour la lecture.");
    }
}
