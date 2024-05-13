#include "gestion_produit.h"
#include "ui_gestion_produit.h"

#include <QMainWindow>
#include <QSortFilterProxyModel>
#include <QTextTableFormat>
#include <QStandardItemModel>
#include <QDialog>
#include <QMessageBox>
#include <QDebug>

gestion_produit::gestion_produit(QWidget *parent) : QMainWindow(parent),
                                                    ui(new Ui::gestion_produit)
{
    ui->setupUi(this);
    // c.createconnect();
    // affichage contenu de la base
    show_tables();

    /////////// arduino
    int ret = A.connect_arduino(); // lancer la connexion Ã  arduino
    switch (ret)
    {
    case (0):
        qDebug() << "arduino is available and connected to : " << A.getarduino_port_name();
        break;
    case (1):
        qDebug() << "arduino is available but not connected to :" << A.getarduino_port_name();
        break;
    case (-1):
        qDebug() << "arduino is not available";
    }
    QObject::connect(A.getserial(), SIGNAL(readyRead()), this, SLOT(update_label())); // permet de lancer
}
gestion_produit::~gestion_produit()
{
    delete ui;
}

//********************************* get variables ************************************

// recuperation des donnees du formulaire
QString gestion_produit::get_type() const
{ // combo box
    return ui->type->currentText();
}
QString gestion_produit::get_id() const
{ // line edit
    return ui->id->text();
}
QString gestion_produit::get_source() const
{ // line edit
    return ui->source->text();
}
QString gestion_produit::get_etat() const
{ // combo box
    return ui->etat->currentText();
}
QString gestion_produit::get_status() const
{ // combo box
    return ui->status->currentText();
}
double gestion_produit::get_prix() const
{ // float
    return ui->prix->value();
}
int gestion_produit::get_quantite() const
{ // float
    return ui->quantite->value();
}

// verification
bool gestion_produit::verif_dechet()
{
    QString error_txt = "";

    QString to_check = ui->id->text();
    if (!(to_check.length() == 5 && to_check.toInt() >= 0))
    {
        error_txt = "error id";
        ui->dechet_error_lb->setText(error_txt);
        return false;
    }

    to_check = ui->source->text();
    QRegularExpression regex("^[A-Za-z]+$");
    if (!(!to_check.trimmed().isEmpty() && regex.match(to_check).hasMatch()))
    {
        error_txt = "error source";
        ui->dechet_error_lb->setText(error_txt);
        return false;
    }

    to_check = ui->type->currentIndex();
    if (to_check < 1)
    {
        error_txt = "error type";
        ui->dechet_error_lb->setText(error_txt);
        return false;
    }

    to_check = QString::number(ui->prix->value());
    if (to_check == "0")
    {
        error_txt = "error prix";
        ui->dechet_error_lb->setText(error_txt);
        return false;
    }

    to_check = ui->etat->currentIndex();
    if (to_check < 1)
    {
        error_txt = "error etat";
        ui->dechet_error_lb->setText(error_txt);
        return false;
    }

    to_check = ui->status->currentIndex();
    if (to_check < 1)
    {
        error_txt = "error satus";
        ui->dechet_error_lb->setText(error_txt);
        return false;
    }

    to_check = QString::number(ui->quantite->value());
    if (to_check == "0")
    {
        error_txt = "error quantite";
        ui->dechet_error_lb->setText(error_txt);
        return false;
    }

    ui->dechet_error_lb->setText(error_txt);
    return true;
}

// remplissage des champs du formulaire a partir de la base
void gestion_produit::fill_form(QString selected)
{
    QSqlQuery query;
    query.prepare("select * from dechet where id= :id_stock");
    query.bindValue(":id_stock", selected);
    query.exec();
    while (query.next())
    {
        ui->id->setText(query.value(0).toString());            // line edit
        ui->source->setText(query.value(2).toString());        // line edit
        ui->type->setCurrentText(query.value(1).toString());   // combobox
        ui->etat->setCurrentText(query.value(4).toString());   // combobox
        ui->status->setCurrentText(query.value(6).toString()); // combobox
        ui->prix->setValue(query.value(5).toReal());           // reel
        ui->quantite->setValue(query.value(3).toInt());        // integer
    }
}

// vider les champs du formulaire
void gestion_produit::clear_form()
{
    ui->id->clear();
    ui->source->clear();
    ui->type->setCurrentIndex(0);
    ui->etat->setCurrentIndex(0);
    ui->status->setCurrentIndex(0);
    ui->quantite->setValue(0);
    ui->prix->setValue(0);
}
//************************************* CRUD ******************************************
// affichage
void gestion_produit::show_tables()
{
    // creation model (masque du tableau) : permet recherche et tri
    proxy = new QSortFilterProxyModel();

    // definir la source (tableau original)
    proxy->setSourceModel(tmp.afficher());

    // pour la recherche
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive); // S=s (pas de difference entre majiscule et miniscule)
    proxy->setFilterKeyColumn(-1);                        // chercher dans tout le tableau (-1) ou donner le numero de la colone
                                                          // remplissage tableau avec le masque
    ui->tableau_produit->setModel(proxy);
}

// ajouter
void gestion_produit::on_ajouter_clicked()
{
    // recuperation des donnees
    QString s1 = get_id();
    QString s2 = get_type();
    QString s3 = get_source();
    QString s4 = get_etat();
    QString s5 = get_status();
    int x = get_quantite();
    double y = get_prix();

    // ajout
    if (verif_dechet())
    {
        produit d(s1, s2, s3, s4, s5, x, y);
        d.ajouter();
        // refresh du tableau (affichage)
        clear_form();
        show_tables();

        // send mail
        /*
        if (s5 == "Livré")
        {
            bool res = mailer.sendMailNormal("chihab.khaled@esprit.tn", "Produit livré !", "le produit a été livré !");
            qDebug() << res;
        }*/
    }
}

// get id of selected row
void gestion_produit::on_tableau_produit_clicked(const QModelIndex &index)
{
    selected = ui->tableau_produit->model()->data(index).toString();
}

// modifier
void gestion_produit::on_tableau_produit_doubleClicked(const QModelIndex &index)
{
    fill_form(selected);
}

void gestion_produit::on_modifier_clicked()
{
    // recuperation des donnees
    QString s1 = selected;
    QString s2 = get_type();
    QString s3 = get_source();
    QString s4 = get_etat();
    QString s5 = get_status();
    int x = get_quantite();
    double y = get_prix();
    // mofication
    if (verif_dechet())
    {
        produit d(selected, s2, s3, s4, s5, x, y);
        d.modifier(selected);
        // refresh du tableau (affichage)
        clear_form();
        show_tables();
    }
}

// supprimer
void gestion_produit::on_supprimer_clicked()
{
    produit d;
    d.supprimer(selected);

    // refresh du tableau (affichage)
    show_tables();
}

//************************************* Metiers ***************************************

// recherche dynamique
void gestion_produit::on_sel_col_currentIndexChanged(int index)
{
    sel_col = ui->sel_col->currentIndex() - 1;
    proxy->setFilterKeyColumn(sel_col); // chercher dans tout le tableau (-1) ou donner le numero de la colone
}
void gestion_produit::on_rech_textChanged(const QString &arg1)
{
    proxy->setFilterFixedString(arg1);
}

// statistique
void gestion_produit::on_stat_clicked()
{
    s = new stat_combo();
    s->setWindowTitle("statistique ComboBox");
    s->choix_pie();
    s->show();
}

// export excel
void gestion_produit::on_exporter_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"), qApp->applicationDirPath(),
                                                    tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
        return;

    ExportExcelObject obj(fileName, "mydata", ui->tableau_produit);

    // colums to export
    obj.addField(0, "Id", "char(20)");
    obj.addField(1, "Type", "char(20)");
    obj.addField(2, "Source", "char(20)");
    obj.addField(3, "Quantite", "char(20)");
    obj.addField(4, "Etat", "char(20)");
    obj.addField(5, "Prix", "char(20)");
    obj.addField(5, "Status", "char(20)");

    int retVal = obj.export2Excel();
    if (retVal > 0)
    {
        QMessageBox::information(this, tr("Done"),
                                 QString(tr("%1 records exported!")).arg(retVal));
    }
}

void gestion_produit::on_pave_2_clicked()
{
    QModelIndexList selectedrows = ui->tableau_produit->selectionModel()->selectedRows();
    if (!selectedrows.isEmpty())
    {
        QModelIndex s = selectedrows.at(0);
        int row = s.row();
        int temp = ui->tableau_produit->model()->index(row, 7).data().toInt();
        qInfo() << "temp : " << temp << endl;
        A.write_to_arduino(QString::number(temp).toUtf8());
    }
    else
        qInfo() << "no data selected : " << endl;
}
