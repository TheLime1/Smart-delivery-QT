//! ******************AYMEN****************** !//
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QTableView>
#include <QDialog>
#include <QSortFilterProxyModel>
#include <QTime>

#include <QTextDocument>
#include <QPrinter>
#include <QTextCursor>
#include <QTextTable>
#include <QFileDialog>
#include <QTextStream>
#include <QPdfWriter>

#include <QDebug>
#include <QSqlError>

#include <QProcess>
#include <QThread>
#include <QGraphicsScene>

#include <QVBoxLayout>
#include <QWidget>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QHBoxLayout>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "commande.h"
#include "dialog.h"
#include "chat_interface.h"
#include "chat.h"
#include "gas.h"
#include "arduino.h"
#include "monitordialog.h"

//! MARIEM
#include "client.h"
#include "mailer.h"
// dora
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "reclamation.h"
#include <QIntValidator>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDateEdit>
#include <QPlainTextEdit>
#include <QPixmap>
#include <QListWidget>
#include <QUrl>
#include <QPdfWriter>
#include <QFile>
#include <QCoreApplication>
#include <QTextStream>
#include <QtPrintSupport/QPrinter>
#include <QFileDialog>
#include <QTextDocument>
#include <QPainter>
#include <QStringList>
#include <QComboBox>
#include <QDesktopServices>
#include <QImage>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow),
                                          md(QString(), this)
{
    //! universal mainwindow
    //!  AYMEN
    ui->setupUi(this);

    ui->day_input_2->setValidator(new QIntValidator(1, 31, this));
    ui->month_input_2->setValidator(new QIntValidator(1, 12, this));
    ui->year_input_2->setValidator(new QIntValidator(0, 9999, this));

    ui->tableView_2->setModel(Ctmp.afficher());
    this->sortState = false;
    this->sortStateRef = false;
    this->sortStateAddress = false;
    ui->tableView_2->setModel(Ctmp.afficher());
    // dorra
    QObject::connect(A.getserial(), SIGNAL(readyRead()), this, SLOT(update_label()));

    ui->la_ref->setValidator(new QIntValidator(0, 99999999, this));
    ui->ref_mod->setValidator(new QIntValidator(0, 99999999, this));

    Reclamation rec;
    QStringList referencesStringList;
    ui->tab_reclamation->setModel(rec.afficher_rec());

    for (const int ref : rec.recupererToutesLesReferences())
    {
        referencesStringList << QString::number(ref);
    }
    ui->comboBox_references->addItems(referencesStringList);

    // arduino stuff
    int ret = uno.connect_arduino();
    switch (ret)
    {
    case 0:
        qDebug() << "arduino is available and connected to : " << uno.getarduino_port_name();
        break;
    case 1:
        qDebug() << "arduino is available but not connected to : " << uno.getarduino_port_name();
        break;
    case -1:
        qDebug() << "arduino is not available";
        break;
    }

    // doraa

    // end
    //   uno.write_to_arduino("start");
    // QObject::connect(uno.getserial(), SIGNAL(readyRead()), this, SLOT(update_label()));

    //! DHIA
    ui->tableView->setModel(Ltmp.afficher());
    connect(ui->PDF, SIGNAL(clicked()), this, SLOT(on_PDF_clicked()));

    //! MARIEM
    ui->le_cin->setValidator(new QIntValidator(00000000, 99999999, this));
    ui->le_cin_2->setValidator(new QIntValidator(00000000, 99999999, this));
    ui->le_numero->setValidator(new QIntValidator(11111111, 99999999, this));
    ui->le_cin_3->setValidator(new QIntValidator(00000000, 99999999, this));
    ui->tab_client->setModel(c.afficher());
    clients_plus_fideles();
}

void MainWindow::update_label()
{
    static QString buffer; // buffer to hold incoming data

    buffer += uno.read_from_arduino(); // append new data to buffer

    int newlineIndex = buffer.indexOf('\n'); // find newline character

    // if newline character found, we have a complete line
    if (newlineIndex != -1)
    {
        QString data = buffer.left(newlineIndex); // get the complete line
        buffer.remove(0, newlineIndex + 1);       // remove the line from the buffer

        qDebug() << "data : " << data;

        // greeting
        QTime current_time = QTime::currentTime();
        int hour = current_time.hour();

        QString greeting;
        if (hour < 12)
        {
            greeting = "Good morning, ";
        }
        else if (hour < 18)
        {
            greeting = "Good afternoon, ";
        }
        else
        {
            greeting = "Good evening, ";
        }

        greeting += "[NOM EMPLOYER], Todays feels like " + data;

        ui->greeting_label->setText(greeting);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setTableViewModel(QAbstractItemModel *model)
{
    ui->tableView_2->setModel(model);
}

Commande &MainWindow::getCtmp()
{
    return Ctmp;
}

// regex patterns
QRegExp addressPattern("^[A-Za-z0-9]+[A-Za-z0-9 ]*$");
QRegExp refPattern("^[A-Za-z0-9]+[A-Za-z0-9 ]*$");
QRegExp prixPattern("^\\d+\\.?\\d*$");
QRegExp datePattern("^([1-9][0-9]*|)$");
#include "client.h"
void MainWindow::on_add_button_3_clicked()
{
    QString ref = ui->ref_input_2->text();
    QString client = ui->client_combo_2->currentText();
    QString livreur = ui->livreur_combo_2->currentText();
    QString adr = ui->address_input_2->text();
    int dd = ui->day_input_2->text().toInt();
    int mm = ui->month_input_2->text().toInt();
    int yyyy = ui->year_input_2->text().toInt();
    float prix = ui->prix_input_2->text().toFloat();
    QString stock_id = ui->stock_combo_2->currentText();
    QString status = ui->status_combo_2->currentText();

    Commande C(ref, livreur, client, adr, dd, mm, yyyy, prix, stock_id, status);
    int cin = client.toInt();
    QString prixString = QString::number(prix);
    QString ddString = QString::number(dd);
    QString mmString = QString::number(mm);
    QString yyyyString = QString::number(yyyy);

    bool test_rech = C.rechercher(ref);
    if (test_rech)
    {
        QMessageBox::critical(nullptr, QObject::tr("NOT OK"),
                              QObject::tr("Commande déjà existante.\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);
        return;
    }
    else if (!refPattern.exactMatch(ref))
    {
        QMessageBox::warning(this, "Erreur", "La référence n'est pas valide.");
        return;
    }
    else if (!addressPattern.exactMatch(adr))
    {
        QMessageBox::warning(this, "Erreur", "L'adresse n'est pas valide.");
        return;
    }
    else if (!datePattern.exactMatch(ddString))
    {
        QMessageBox::warning(this, "Erreur", "Le jour n'est pas valide.");
        return;
    }
    else if (!datePattern.exactMatch(mmString))
    {
        QMessageBox::warning(this, "Erreur", "Le mois n'est pas valide.");
        return;
    }
    else if (!datePattern.exactMatch(yyyyString))
    {
        QMessageBox::warning(this, "Erreur", "L'année n'est pas valide.");
        return;
    }
    else if (!prixPattern.exactMatch(prixString)) // must be string to check
    {
        QMessageBox::warning(this, "Erreur", "Le prix n'est pas valide.");
        return;
    }

    else
    {
        bool test_ajout = C.ajouter();
        if (test_ajout)
        {
            // refresh
            ui->tableView_2->setModel(Ctmp.afficher());

            QMessageBox::information(nullptr, QObject::tr("Success"),
                                     QObject::tr("Commande ajoutée."),
                                     QMessageBox::Ok);
        }
        else
            QMessageBox::critical(nullptr, QObject::tr("NOT OK"),
                                  QObject::tr("Commande non ajoutée.\n"
                                              "Click Cancel to exit."),
                                  QMessageBox::Cancel);
    }
    int pointsFidelite;
    bool test = C.ajouter();
    if (test)
    {
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("ajout effectué.\n"
                                             "Click Cancel to exit."),
                                 QMessageBox::Cancel);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("NOT OK"),
                              QObject::tr("ajout failed.\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);
    }
    if (c.existe(cin))
    {
        QSqlQuery query;
        query.prepare("UPDATE CLIENT SET POINTSFIDELITE = POINTSFIDELITE + 10 WHERE CIN = :cin");
        query.bindValue(":cin", cin);

        if (query.exec())
        {
            qDebug() << "Mise à jour des points de fidélité effectuée avec succès.";
        }
        else
        {
            qDebug() << "Erreur lors de la mise à jour des points de fidélité : " << query.lastError().text();
        }
    }
}

void MainWindow::on_delete_button_2_clicked()
{
    QString ref = ui->delete_input_2->text();
    bool test_rech = Ctmp.rechercher(ref);
    if (test_rech)
    {
        bool test_delete = Ctmp.supprimer(ref);
        if (test_delete)
        {
            // refresh
            ui->tableView_2->setModel(Ctmp.afficher());

            QMessageBox::information(nullptr, QObject::tr("OK"),
                                     QObject::tr("Commande supprimée.\n"
                                                 "Click Cancel to exit."),
                                     QMessageBox::Cancel);
        }
        else
            QMessageBox::critical(nullptr, QObject::tr("NOT OK"),
                                  QObject::tr("Commande non supprimée.\n"
                                              "Click Cancel to exit."),
                                  QMessageBox::Cancel);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("NOT OK"),
                              QObject::tr("Impossible de trouver de référence\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);
    }
}

void MainWindow::on_cherch_button_2_clicked()
{
    QString ref = ui->cherch_input_2->text();
    Commande c;
    bool test = c.rechercher(ref);

    if (test)
    {
        Dialog d(this);
        d.afficherref(ref);
        d.exec();
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("whoops"),
                              QObject::tr("Impossible de trouver de référence\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);
    }
}

// when i click afficher widget it refrech
void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    if (index == 1)
    {
        ui->tableView_2->setModel(Ctmp.afficher());
    }
}

void MainWindow::on_sortButton_2_clicked()
{
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(Ctmp.afficher());

    if (this->sortState)
    { // 4 for day //5 for month //6 for year
        proxyModel->sort(6, Qt::DescendingOrder);
    }
    else
    {
        proxyModel->sort(6, Qt::AscendingOrder);
    }
    sortState = !sortState;

    ui->tableView_2->setModel(proxyModel);
}

void MainWindow::on_exportPDFbutton_2_clicked()
{
    // Select the location to save the PDF file
    QString filePath = QFileDialog::getSaveFileName(this, "Save PDF", "", "PDF Files (*.pdf)");

    if (!filePath.isEmpty())
    {
        // Create a QPdfWriter object with the path of the PDF file
        QPdfWriter pdfWriter(filePath);
        pdfWriter.setPageSizeMM(QSizeF(297, 210)); //

        // Open the PDF document for writing
        QPainter painter(&pdfWriter);
        if (!painter.isActive())
        {
            return;
        }
        // Retrieve the supplier data
        QSqlQueryModel *model = Ctmp.afficher();
        if (!model)
        {
            return;
        }

        // Define the margins of the page
        const qreal leftMargin = 20;
        const qreal topMargin = 20;
        const qreal rightMargin = 20;
        const qreal bottomMargin = 20;

        pdfWriter.setPageMargins(QMarginsF(leftMargin, topMargin, rightMargin, bottomMargin));

        // Retrieve the margins of the page
        const QMarginsF pageMargins = pdfWriter.pageLayout().margins();
        const qreal pageLeftMargin = pageMargins.left();
        const qreal pageTopMargin = pageMargins.top();
        const qreal pageRightMargin = pageMargins.right();
        const qreal pageBottomMargin = pageMargins.bottom();

        // Calculate the width and height available for the table
        const qreal tableWidth = pdfWriter.width() - pageLeftMargin - pageRightMargin;
        const qreal tableHeight = pdfWriter.height() - pageTopMargin - pageBottomMargin;

        // Define the properties of the table
        const int rowCount = model->rowCount();
        const int columnCount = model->columnCount();
        const qreal lineHeight = 300;   // Reduced line height to decrease vertical space
        const qreal columnWidth = 2000; // Equal column width for each item
        const QFont font("Arial", 10);
        const QFont headerFont("Arial", 10, QFont::Bold); // Bold font for headers

        // Draw the table
        qreal x = pageLeftMargin;
        qreal y = pageTopMargin;

        // Draw the title
        QFont titleFont("Arial", 20, QFont::Bold);
        painter.setFont(titleFont);
        painter.drawText(QRectF(pageLeftMargin, y, tableWidth, lineHeight), Qt::AlignCenter, "Liste des Commandes");

        // Adjust y coordinate to leave space for the title
        y += 2 * lineHeight;

        // Draw the column headers
        painter.setFont(headerFont); // Use bold font for headers
        for (int column = 0; column < columnCount; ++column)
        {
            QRectF cellRect(x, y, columnWidth, lineHeight);
            painter.drawText(cellRect, Qt::AlignCenter, model->headerData(column, Qt::Horizontal).toString());
            x += columnWidth;
        }

        y += lineHeight;

        // Draw the supplier data
        painter.setFont(font); // Switch back to normal font for data
        for (int row = 0; row < rowCount; ++row)
        {
            x = pageLeftMargin;

            for (int column = 0; column < columnCount; ++column)
            {
                QRectF cellRect(x, y, columnWidth, lineHeight);
                painter.drawText(cellRect, Qt::AlignLeft | Qt::AlignVCenter, model->data(model->index(row, column)).toString());
                x += columnWidth;
            }

            y += lineHeight;

            // Draw a horizontal line
            painter.setPen(QPen(Qt::black, 1));
            painter.drawLine(pageLeftMargin, y, pageLeftMargin + tableWidth, y);
        }

        // Close the PDF document
        painter.end();
    }
}

void MainWindow::on_refrechButton_clicked()
{
    // Refresh afficher
    ui->tableView_2->setModel(Ctmp.afficher());

    // Refresh combo box client
    QSqlQuery queryClient;
    queryClient.exec("SELECT CIN FROM CLIENT");
    ui->client_combo_2->clear();
    while (queryClient.next())
    {
        QString clientId = queryClient.value(0).toString();
        ui->client_combo_2->addItem(clientId);
    }

    // Refresh combo box livreur
    QSqlQuery queryLivreur;
    queryLivreur.exec("SELECT REFERENCE FROM LIVREUR");
    ui->livreur_combo_2->clear();
    while (queryLivreur.next())
    {
        QString livreurId = queryLivreur.value(0).toString();
        ui->livreur_combo_2->addItem(livreurId);
    }

    // Refresh combo box stock
    QSqlQuery queryStock;
    queryStock.exec("SELECT ID_STOCK FROM PRODUIT");
    ui->stock_combo_2->clear();
    while (queryStock.next())
    {
        QString stockId = queryStock.value(0).toString();
        ui->stock_combo_2->addItem(stockId);
    }

    // Refresh combo box status
    ui->status_combo_2->addItem("delivered");
    ui->status_combo_2->addItem("not delivered");

    // refrech monitor (undelivered commands)
    // Clear the grid layout first
    QLayoutItem *item;
    while ((item = ui->grid_2->takeAt(0)) != nullptr)
    {
        delete item->widget();
        delete item;
    }

    // refresh monitor (undelivered commands)
    QSqlQuery query;
    query.prepare("SELECT COMMANDE.REFERENCE, PRODUIT.TYPE FROM COMMANDE "
                  "INNER JOIN PRODUIT ON COMMANDE.STOCK_ID = PRODUIT.ID_STOCK "
                  "WHERE COMMANDE.STATUS = 'not delivered'");

    if (query.exec())
    {
        int column = 0;
        while (query.next())
        {
            QString id = query.value(0).toString();
            QString stockType = query.value(1).toString();

            // Create a group box
            QGroupBox *groupBox = new QGroupBox(this);

            // Create a layout for the group box
            QVBoxLayout *layout = new QVBoxLayout;

            // Create a label with the id and add it to the layout
            QLabel *idLabel = new QLabel("ID: " + id);
            layout->addWidget(idLabel);

            // Create a label with the stock type and add it to the layout
            QLabel *stockTypeLabel = new QLabel("Stock Type: " + stockType);
            layout->addWidget(stockTypeLabel);

            // Create a button and add it to the layout
            QPushButton *button = new QPushButton("Monitor");
            button->setProperty("id", id); // Store the id in the button's properties
            connect(button, &QPushButton::clicked, this, &MainWindow::onMonitorButtonClicked);
            layout->addWidget(button);

            // Set the group box's layout
            groupBox->setLayout(layout);

            // Calculate the row and column for the grid layout
            int row = column / 4;

            // Add the group box to the grid layout at the calculated position
            ui->grid_2->addWidget(groupBox, row, column % 4);

            // Increment the column counter
            column++;
        }
    }
    else
    {
        qDebug() << "Failed to execute query: " << query.lastError();
    }
}

void MainWindow::onMonitorButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button)
    {
        QString id = button->property("id").toString();
        qDebug() << "Monitor button clicked for id: " << id;

        uno.close_arduino();
        md.setId(id); // Set the id of md
        md.exec();    // Show md as a modal dialog
    }
}

void MainWindow::on_map_button_clicked()
{
    QSqlQuery query;
    bool execSuccess = query.exec("SELECT DESTINATION FROM COMMANDE");

    if (!execSuccess)
    {
        qDebug() << "Query execution failed: " << query.lastError().text();
        return;
    }

    // Workaround to get the number of rows //!SQL error
    int numberOfRows = 0;
    if (query.last())
    {
        numberOfRows = query.at() + 1;
        query.first();
        query.previous();
    }

    qDebug() << "Number of rows: " << numberOfRows;

    QString filePath = "C:/Users/everp/Documents/GitHub/2a23-smart_delivery/map/address.txt";
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file";
        return;
    }

    QTextStream out(&file);

    while (query.next())
    {
        QString destination = query.value(0).toString();
        qDebug() << "Writing destination: " << destination;
        out << destination << "\n";
    }

    file.close();
    qDebug() << "File written successfully";

    // Run the Python script
    QString pythonScriptPath = "C:/Users/everp/Documents/GitHub/2a23-smart_delivery/map/main.py";
    QProcess pythonProcess;
    pythonProcess.setWorkingDirectory("C:/Users/everp/Documents/GitHub/2a23-smart_delivery/map");
    pythonProcess.start("C:/Users/everp/AppData/Local/Programs/Python/Python311/python.exe", QStringList() << pythonScriptPath);

    // Wait for the Python script to finish and generate the image
    if (!pythonProcess.waitForFinished(-1)) // wait indefinitely until finished
    {
        qDebug() << "Python script execution failed with error:" << pythonProcess.errorString();
    }
    else
    {
        qDebug() << "Python script output:" << pythonProcess.readAllStandardOutput();
        qDebug() << "Python script errors:" << pythonProcess.readAllStandardError();
    }

    // Wait for the Python script to finish and generate the image
    QThread::sleep(2); //? Adjust the sleep time as needed

    // Load the image
    QString imagePath = "C:/Users/everp/Documents/GitHub/2a23-smart_delivery/map/map.png";
    QPixmap pixmap(imagePath);

    // Create a QGraphicsScene, add the image to it
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->addPixmap(pixmap);

    // Set the scene for the QGraphicsView
    ui->map_pic->setScene(scene);

    // Fit the image in the QGraphicsView
    ui->map_pic->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::on_admin_dummy_button_clicked()
{
    Ctmp.add_dummy_data();
    ui->tableView_2->setModel(Ctmp.afficher());

    // refrech monitor (undelivered commands)
    // Clear the grid layout first
    QLayoutItem *item;
    while ((item = ui->grid_2->takeAt(0)) != nullptr)
    {
        delete item->widget();
        delete item;
    }

    // refresh monitor (undelivered commands)
    QSqlQuery query;
    query.prepare("SELECT COMMANDE.REFERENCE, PRODUIT.TYPE FROM COMMANDE "
                  "INNER JOIN PRODUIT ON COMMANDE.STOCK_ID = PRODUIT.ID_STOCK "
                  "WHERE COMMANDE.STATUS = 'not delivered'");

    if (query.exec())
    {
        int column = 0;
        while (query.next())
        {
            QString id = query.value(0).toString();
            QString stockType = query.value(1).toString();

            // Create a group box
            QGroupBox *groupBox = new QGroupBox(this);

            // Create a layout for the group box
            QVBoxLayout *layout = new QVBoxLayout;

            // Create a label with the id and add it to the layout
            QLabel *idLabel = new QLabel("ID: " + id);
            layout->addWidget(idLabel);

            // Create a label with the stock type and add it to the layout
            QLabel *stockTypeLabel = new QLabel("Stock Type: " + stockType);
            layout->addWidget(stockTypeLabel);

            // Create a button and add it to the layout
            QPushButton *button = new QPushButton("Monitor");
            button->setProperty("id", id); // Store the id in the button's properties
            connect(button, &QPushButton::clicked, this, &MainWindow::onMonitorButtonClicked);
            layout->addWidget(button);

            // Set the group box's layout
            groupBox->setLayout(layout);

            // Calculate the row and column for the grid layout
            int row = column / 4;

            // Add the group box to the grid layout at the calculated position
            ui->grid_2->addWidget(groupBox, row, column % 4);

            // Increment the column counter
            column++;
        }
    }
    else
    {
        qDebug() << "Failed to execute query: " << query.lastError();
    }
}

void MainWindow::on_admin_remove_button_clicked()
{
    Ctmp.deleteall();
    ui->tableView_2->setModel(Ctmp.afficher());

    // refrech monitor (undelivered commands)
    // Clear the grid layout first
    QLayoutItem *item;
    while ((item = ui->grid_2->takeAt(0)) != nullptr)
    {
        delete item->widget();
        delete item;
    }

    // refresh monitor (undelivered commands)
    QSqlQuery query;
    query.prepare("SELECT COMMANDE.REFERENCE, PRODUIT.TYPE FROM COMMANDE "
                  "INNER JOIN PRODUIT ON COMMANDE.STOCK_ID = PRODUIT.ID_STOCK "
                  "WHERE COMMANDE.STATUS = 'not delivered'");

    if (query.exec())
    {
        int column = 0;
        while (query.next())
        {
            QString id = query.value(0).toString();
            QString stockType = query.value(1).toString();

            // Create a group box
            QGroupBox *groupBox = new QGroupBox(this);

            // Create a layout for the group box
            QVBoxLayout *layout = new QVBoxLayout;

            // Create a label with the id and add it to the layout
            QLabel *idLabel = new QLabel("ID: " + id);
            layout->addWidget(idLabel);

            // Create a label with the stock type and add it to the layout
            QLabel *stockTypeLabel = new QLabel("Stock Type: " + stockType);
            layout->addWidget(stockTypeLabel);

            // Create a button and add it to the layout
            QPushButton *button = new QPushButton("Monitor");
            button->setProperty("id", id); // Store the id in the button's properties
            connect(button, &QPushButton::clicked, this, &MainWindow::onMonitorButtonClicked);
            layout->addWidget(button);

            // Set the group box's layout
            groupBox->setLayout(layout);

            // Calculate the row and column for the grid layout
            int row = column / 4;

            // Add the group box to the grid layout at the calculated position
            ui->grid_2->addWidget(groupBox, row, column % 4);

            // Increment the column counter
            column++;
        }
    }
    else
    {
        qDebug() << "Failed to execute query: " << query.lastError();
    }
}

void MainWindow::on_sortButton_ref_2_clicked()
{
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(Ctmp.afficher());

    if (this->sortStateRef)
    {
        proxyModel->sort(0, Qt::DescendingOrder);
    }
    else
    {
        proxyModel->sort(0, Qt::AscendingOrder);
    }
    sortStateRef = !sortStateRef;

    ui->tableView_2->setModel(proxyModel);
}

void MainWindow::on_sortButton_address_3_clicked()
{
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(Ctmp.afficher());

    if (this->sortStateAddress)
    {
        proxyModel->sort(3, Qt::DescendingOrder);
    }
    else
    {
        proxyModel->sort(3, Qt::AscendingOrder);
    }
    sortStateAddress = !sortStateAddress;

    ui->tableView_2->setModel(proxyModel);
}

void MainWindow::on_live_search_22_textChanged(const QString &arg1)
{
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(Ctmp.afficher());
    proxyModel->setFilterKeyColumn(3);
    QRegExp regex(arg1, Qt::CaseInsensitive, QRegExp::FixedString);
    proxyModel->setFilterRegExp(regex);
    ui->tableView_2->setModel(proxyModel);
}

void MainWindow::on_live_search_ref_2_textChanged(const QString &arg1)
{
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(Ctmp.afficher());
    proxyModel->setFilterKeyColumn(0);
    QRegExp regex(arg1, Qt::CaseInsensitive, QRegExp::FixedString);
    proxyModel->setFilterRegExp(regex);
    ui->tableView_2->setModel(proxyModel);
}

void MainWindow::on_chat_pop_button_2_clicked()
{
    Ctmp.sql_csv();
    ch.clean_csv();
    ch.csv_line();
    chat_interface chui(this);
    chui.exec();
}
QtCharts::QChart *MainWindow::createBarChart(QAbstractItemModel *model)
{
    QMap<QString, int> counts;
    int totalCount = 0;
    for (int row = 0; row < model->rowCount(); ++row)
    {
        QString value = model->index(row, 1).data().toString();
        counts[value]++;
        totalCount++;
    }

    QtCharts::QBarSeries *series = new QtCharts::QBarSeries();
    for (auto it = counts.begin(); it != counts.end(); ++it)
    {
        QtCharts::QBarSet *set = new QtCharts::QBarSet(it.key());
        *set << it.value();
        double percentage = 100.0 * it.value() / totalCount;
        set->setLabel(QString("%1 (%2 deliveries, %3%)").arg(it.key()).arg(it.value()).arg(percentage));
        series->append(set);
    }

    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->addSeries(series);
    chart->setTitle("Number of Deliveries by Each Livreur");
    return chart;
}

QtCharts::QChart *MainWindow::createPieChart(QAbstractItemModel *model)
{
    QMap<QString, int> counts;
    int totalCount = 0;
    for (int row = 0; row < model->rowCount(); ++row)
    {
        QString value = model->index(row, 9).data().toString();
        counts[value]++;
        totalCount++;
    }

    QtCharts::QPieSeries *series = new QtCharts::QPieSeries();
    for (auto it = counts.begin(); it != counts.end(); ++it)
    {
        QtCharts::QPieSlice *slice = series->append(it.key(), it.value());
        double percentage = 100.0 * it.value() / totalCount;
        slice->setLabel(QString("%1%2 %3%").arg(it.key()).arg(": ").arg(percentage));
    }

    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->addSeries(series);
    chart->setTitle("Status of the Commands");
    return chart;
}

void MainWindow::on_stats_button_2_clicked()
{
    QAbstractItemModel *model = ui->tableView_2->model();
    QtCharts::QChart *pieChart = createPieChart(model);
    QtCharts::QChart *barChart = createBarChart(model);

    QtCharts::QChartView *pieChartView = new QtCharts::QChartView(pieChart);
    pieChartView->setRenderHint(QPainter::Antialiasing);

    QtCharts::QChartView *barChartView = new QtCharts::QChartView(barChart);
    barChartView->setRenderHint(QPainter::Antialiasing);

    QWidget *widget = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->addWidget(pieChartView);
    layout->addWidget(barChartView);
    widget->resize(1600, 600);
    widget->show();
}

void MainWindow::on_clear_button_3_clicked()
{
    ui->ref_input_2->clear();
    ui->address_input_2->clear();
    ui->day_input_2->clear();
    ui->month_input_2->clear();
    ui->year_input_2->clear();
    ui->prix_input_2->clear();
}

void MainWindow::on_gas_button_clicked()
{
    // get the delivred address
    QSqlQuery query;
    bool execSuccess = query.exec("SELECT DESTINATION FROM COMMANDE WHERE STATUS = 'delivered'");

    if (!execSuccess)
    {
        qDebug() << "Query execution failed: " << query.lastError().text();
        return;
    }

    // Workaround to get the number of rows //!SQL error
    int numberOfRows = 0;
    if (query.last())
    {
        numberOfRows = query.at() + 1;
        query.first();
        query.previous();
    }

    qDebug() << "Number of rows: " << numberOfRows;

    QString filePath = "C:/Users/everp/Documents/GitHub/2a23-smart_delivery/gas/addressD.txt";
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file";
        return;
    }

    QTextStream out(&file);

    while (query.next())
    {
        QString destination = query.value(0).toString();
        qDebug() << "Writing destination: " << destination;
        out << destination << "\n";
    }

    file.close();
    qDebug() << "File written successfully";

    file.close();
    qDebug() << "File written successfully";

    // Run the xy.py script
    QString pythonScriptPath = "C:/Users/everp/Documents/GitHub/2a23-smart_delivery/gas/xy.py";
    QProcess pythonProcess;
    pythonProcess.setWorkingDirectory("C:/Users/everp/Documents/GitHub/2a23-smart_delivery/gas");
    pythonProcess.start("C:/Users/everp/AppData/Local/Programs/Python/Python311/python.exe", QStringList() << pythonScriptPath);

    // Wait for the Python script to finish
    if (!pythonProcess.waitForFinished(-1)) // wait indefinitely until finished
    {
        qDebug() << "Python script execution failed with error:" << pythonProcess.errorString();
    }
    else
    {
        qDebug() << "Python script output:" << pythonProcess.readAllStandardOutput();
        qDebug() << "Python script errors:" << pythonProcess.readAllStandardError();
    }

    QThread::sleep(2);

    // Run the gas.py script
    pythonScriptPath = "C:/Users/everp/Documents/GitHub/2a23-smart_delivery/gas/gas.py";
    QProcess pythonProcess2;
    pythonProcess2.setWorkingDirectory("C:/Users/everp/Documents/GitHub/2a23-smart_delivery/gas");
    pythonProcess2.start("C:/Users/everp/AppData/Local/Programs/Python/Python311/python.exe", QStringList() << pythonScriptPath);

    // Wait for the Python script to finish
    if (!pythonProcess2.waitForFinished(-1)) // wait indefinitely until finished
    {
        qDebug() << "Python script execution failed with error:" << pythonProcess2.errorString();
    }
    else
    {
        qDebug() << "Python script output:" << pythonProcess2.readAllStandardOutput();
        qDebug() << "Python script errors:" << pythonProcess2.readAllStandardError();
    }

    // print gas cost
    double cost = g.calc_cost();
    QString costStr = QString::number(cost, 'f', 2); // Convert float to QString with 2 decimal places
    ui->gas_cost->setText(costStr + " DT saved");    // Set the text of the label

    // print the distance
    double distance = g.calc_distance();
    QString distanceStr = QString::number(distance, 'f', 2); // Convert float to QString with 2 decimal places
    ui->sum_distance->setText(distanceStr + " km traveled"); // Set the text of the label

    // print the CO2 emission
    float CO2 = g.calc_CO2();
    QString CO2Str = QString::number(CO2, 'f', 2); // Convert float to QString with 2 decimal places
    ui->co2->setText(CO2Str + " kg CO2 saved");
}
//! ******************END AYMEN****************** !//

//! ******************KHALED****************** !//
#include "gestion_produit.h"
void MainWindow::on_pushButton_clicked()
{

    close();
    gestion_produit *gd;
    gd = new gestion_produit();
    gd->show();
}
//! ******************END KHALED****************** !//
//! ******************DHIA****************** !//
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "livreur.h"
#include "sms.h"
#include <QFileDialog>
#include <QPdfWriter>
#include <QPainter>
#include <QSortFilterProxyModel>

void MainWindow::on_pushButton_ajouter_livreur_clicked()
{
    QString reference = ui->lineEdit_reference_2->text();
    int autonomie = ui->lineEdit_autonomie_2->text().toInt();
    int capacite = ui->lineEdit_capacite_2->text().toInt();
    int vitesse = ui->lineEdit_vitesse_2->text().toInt();
    QString etat = ui->lineEdit_etat_2->text();

    if (reference.isEmpty() || autonomie <= 0 || capacite <= 0 || vitesse <= 0)
    {
        QMessageBox::critical(this, QObject::tr("Input Error"),
                              QObject::tr("Veuillez remplir tous les champs avec des valeurs valides."), QMessageBox::No);
    }
    else
    {
        Livreur livreur(reference, autonomie, capacite, vitesse, etat);

        bool test = livreur.ajouter();

        if (test)
        {
            QMessageBox::information(this, QObject::tr("OK"),
                                     QObject::tr("Ajout effectué\nClick Cancel to exit."), QMessageBox::Cancel);
            ui->tableView->setModel(Ltmp.afficher());
        }
        else
        {
            QMessageBox::critical(this, QObject::tr("Not OK"),
                                  QObject::tr("Ajout non effectué.\nClick Cancel to exit."), QMessageBox::Cancel);
        }
        sms s;
        s.sendSMS("+21642253001", "livreur ajouté avec succès   reference: " + reference + " | autonomie: " + autonomie + " | vitesse: " + vitesse + " | capacite: " + capacite + "| etat: " + etat);
    }
}

void MainWindow::on_pushButton_livreur_clicked()
{
    Livreur L1;
    L1.setReference(ui->lineEdit_referenceS_2->text());
    bool test = L1.supprimer(L1.getReference());
    if (test)
    {
        QMessageBox::information(this, QObject::tr("OK"),
                                 QObject::tr("Suppression effectué\nClick Cancel to exit."), QMessageBox::Ok);
        ui->tableView->setModel(Ltmp.afficher());
    }
    else
    {
        QMessageBox::critical(this, QObject::tr("Not OK"),
                              QObject::tr("Suppression non effectué.\nClick Cancel to exit."), QMessageBox::Cancel);
    }
}

void MainWindow::on_pushButton_modifier_livreur_clicked()
{
    QString reference = ui->lineEdit_referenceM_2->text();
    int nouvelleAutonomie = ui->lineEdit_nouvelleAutonomie_2->text().toInt();
    int nouvelleCapacite = ui->lineEdit_nouvelleCapacite_2->text().toInt();
    int nouvelleVitesse = ui->lineEdit_nouvelleVitesse_2->text().toInt();
    QString nouvelleEtat = ui->lineEdit_nouvelleEtat_2->text();

    Livreur livreur;
    bool success = livreur.modifier(reference, nouvelleAutonomie, nouvelleCapacite, nouvelleVitesse, nouvelleEtat);

    if (success)
    {
        QMessageBox::information(this, QObject::tr("Modification réussie"),
                                 QObject::tr("Les données ont été mises à jour avec succès."), QMessageBox::Ok);
        ui->tableView->setModel(Ltmp.afficher());
    }
    else
    {
        QMessageBox::critical(this, QObject::tr("Échec de la modification"),
                              QObject::tr("La modification des données a échoué."), QMessageBox::Cancel);
    }
}

void MainWindow::on_pushButton_assigner_livreur_clicked()
{
    QString referenceLivreur = ui->lineEdit_referenceA_2->text();
    QString referenceCommande = ui->lineEdit_colis_2->text();

    Livreur livreur;
    bool success = livreur.assignerLivraison(referenceLivreur, referenceCommande);

    if (success)
    {
        QMessageBox::information(this, "Attribution réussie", "La livraison a été attribuée avec succès.");
    }
    else
    {
        QMessageBox::warning(this, "Erreur", "Le livreur n'est pas disponible ou l'attribution a échoué.");
    }
}

void MainWindow::on_PDF_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer PDF", "", "Fichiers PDF (*.pdf)");

    if (!filePath.isEmpty())
    {

        QPdfWriter pdfWriter(filePath);
        pdfWriter.setPageSizeMM(QSizeF(297, 210)); //

        QPainter painter(&pdfWriter);
        if (!painter.isActive())
        {
            return;
        }

        Livreur livreur;
        QSqlQueryModel *model = livreur.afficher();
        if (!model)
        {
            return;
        }
        // Définir les marges de la page
        const qreal leftMargin = 20;
        const qreal topMargin = 20;
        const qreal rightMargin = 20;
        const qreal bottomMargin = 20;

        pdfWriter.setPageMargins(QMarginsF(leftMargin, topMargin, rightMargin, bottomMargin));

        // Récupérer les marges de la page
        const QMarginsF pageMargins = pdfWriter.pageLayout().margins();
        const qreal pageLeftMargin = pageMargins.left();
        const qreal pageTopMargin = pageMargins.top();
        const qreal pageRightMargin = pageMargins.right();
        const qreal pageBottomMargin = pageMargins.bottom();

        // Calculer la largeur et la hauteur disponibles pour le tableau
        const qreal tableWidth = pdfWriter.width() - pageLeftMargin - pageRightMargin;
        const qreal tableheight = pdfWriter.height() - pageTopMargin - pageBottomMargin;

        // Définir les propriétés du tableau
        const int rowCount = model->rowCount();
        const int columnCount = model->columnCount();
        const qreal lineHeight = 300;   // Hauteur de ligne réduite pour diminuer l'espace vertical
        const qreal columnWidth = 2000; // Largeur de colonne égale pour chaque élément
        const QFont font("Arial", 10);

        // Dessiner le tableau
        qreal x = pageLeftMargin;
        qreal y = pageTopMargin;

        // Dessiner les en-têtes de colonne
        for (int column = 0; column < columnCount; ++column)
        {
            QRectF cellRect(x, y, columnWidth, lineHeight);
            painter.setFont(font);
            painter.drawText(cellRect, Qt::AlignCenter, model->headerData(column, Qt::Horizontal).toString());
            x += columnWidth;
        }

        y += lineHeight;

        // Dessiner les données du fournisseur
        for (int row = 0; row < rowCount; ++row)
        {
            x = pageLeftMargin;

            for (int column = 0; column < columnCount; ++column)
            {
                QRectF cellRect(x, y, columnWidth, lineHeight);
                painter.setFont(font);
                painter.drawText(cellRect, Qt::AlignLeft | Qt::AlignVCenter, model->data(model->index(row, column)).toString());
                x += columnWidth;
            }

            y += lineHeight;

            // Dessiner un trait horizontal
            painter.setPen(QPen(Qt::black, 1));
            painter.drawLine(pageLeftMargin, y, pageLeftMargin + tableWidth, y);
        }
        painter.end();

        QMessageBox::information(this, "PDF généré", "Le fichier PDF a été généré avec succès.");
    }
}

void MainWindow::on_comboBox_livreur_currentIndexChanged(const QString &arg1)
{
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(ui->tableView->model());

    if (arg1 == "Reference")
    {
        proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
        proxyModel->setSortRole(Qt::EditRole);
        proxyModel->sort(0, Qt::AscendingOrder); // Trier par la colonne de référence
    }
    else if (arg1 == "Capacite")
    {
        proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
        proxyModel->setSortRole(Qt::EditRole);
        proxyModel->sort(1, Qt::AscendingOrder); // Trier par la colonne de capacité
    }
    else if (arg1 == "Autonomie")
    {
        proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
        proxyModel->setSortRole(Qt::EditRole);
        proxyModel->sort(2, Qt::AscendingOrder); // Trier par la colonne d'autonomie
    }
    else if (arg1 == "Vitesse")
    {
        proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
        proxyModel->setSortRole(Qt::EditRole);
        proxyModel->sort(3, Qt::AscendingOrder); // Trier par la colonne de vitesse
    }
    else if (arg1 == "Etat")
    {
        proxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);
        proxyModel->setSortRole(Qt::EditRole);
        proxyModel->sort(4, Qt::DescendingOrder); // Trier par la colonne d'état
    }

    ui->tableView->setModel(proxyModel);
}

void MainWindow::on_pushButton_rechercher_livreur_clicked()
{
    QString ref = ui->lineEdit_rechercher_2->text();

    Livreur L1;
    bool test = L1.rechercher(ref);

    if (test)
    {
        QMessageBox::information(this, QObject::tr("OK"),
                                 QObject::tr("Livreur trouvé.\nClick Cancel to exit."), QMessageBox::Cancel);
        ui->tableView->setModel(L1.afficher());
    }
    else
    {
        QMessageBox::critical(this, QObject::tr("Not OK"),
                              QObject::tr("Livreur non trouvé.\nClick Cancel to exit."), QMessageBox::Cancel);
    }
}

void MainWindow::on_pushButton_rechercher_livreur_2_clicked()
{
    int cap = ui->lineEdit_rechercherC_2->text().toInt();
    Livreur L1;
    bool test = L1.rechercherCapacite(cap);

    if (test)
    {
        QMessageBox::information(this, QObject::tr("OK"),
                                 QObject::tr("Livreur trouvé.\nClick Cancel to exit."), QMessageBox::Cancel);
        ui->tableView->setModel(L1.afficher());
    }
    else
    {
        QMessageBox::critical(this, QObject::tr("Not OK"),
                              QObject::tr("Livreur non trouvé.\nClick Cancel to exit."), QMessageBox::Cancel);
    }
}

void MainWindow::on_pushButton_rechercher_livreur_3_clicked()
{
    int aut = ui->lineEdit_rechercherA_2->text().toInt();
    Livreur L1;
    bool test = L1.rechercherAutonomie(aut);

    if (test)
    {
        QMessageBox::information(this, QObject::tr("OK"),
                                 QObject::tr("Livreur trouvé.\nClick Cancel to exit."), QMessageBox::Cancel);
        ui->tableView->setModel(L1.afficher());
    }
    else
    {
        QMessageBox::critical(this, QObject::tr("Not OK"),
                              QObject::tr("Livreur non trouvé.\nClick Cancel to exit."), QMessageBox::Cancel);
    }
}

//! ******************END DHIA****************** !//

//! ******************MARIEM****************** !//
#include <QCoreApplication>
#include <QProcess>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QIntValidator>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QSqlQuery>
#include <QSqlError>
#include <QtDebug>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>
#include <QTextOption>
#include <QFont>
#include <QMap>
#include <QString>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QtMath>
#include <QSqlRecord>
#include <QSqlField>
#include <QMainWindow>
#include <QTableWidgetItem>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
void MainWindow::on_pb_ajouter_client_clicked()
{
    int cin = ui->le_cin->text().toInt();
    QString nom = ui->le_nom->text();
    QString prenom = ui->le_prenom->text();
    QString adresse = ui->le_adresse->text();
    int numero = ui->le_numero->text().toInt();
    QString mail = ui->le_mail->text();
    QString genre = ui->le_genre->text();
    QString date_naissance = ui->le_date->text();

    Client c(cin, nom, prenom, adresse, numero, mail, genre, date_naissance);

    if (c.existe(cin))
    {
        QMessageBox::warning(this, "Erreur", "Le client avec le CIN spécifié existe déjà.");
        return;
    }

    QRegExp emailPattern("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,4}$");
    if (!emailPattern.exactMatch(mail))
    {
        QMessageBox::warning(this, "Erreur", "L'adresse e-mail n'est pas valide.");
        return;
    }

    if (genre != "Male" && genre != "Femme")
    {
        QMessageBox::warning(this, "Erreur", "Le genre doit être 'Male' ou 'Femme'.");
        return;
    }

    QRegExp datePattern("\\d{2}/\\d{2}/\\d{4}"); // Un motif régulier pour jj/mm/année
    if (datePattern.exactMatch(date_naissance))
    {
        QStringList dateParts = date_naissance.split('/');
        int jj = dateParts[0].toInt();
        int mm = dateParts[1].toInt();

        if (jj >= 1 && jj <= 31 && mm >= 1 && mm <= 12)
        {
            // La date est valide
        }
        else
        {
            QMessageBox::warning(this, "Erreur", "La date de naissance doit avoir un jour (jj) entre 1 et 31 et un mois (mm) entre 1 et 12.");
            return;
        }
    }
    else
    {
        QMessageBox::warning(this, "Erreur", "La date de naissance doit être au format jj/mm/année.");
        return;
    }

    bool test = c.ajouter();
    if (test)
    {
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("ajout effectué.\n"
                                             "Click Cancel to exit."),
                                 QMessageBox::Cancel);
    }
    else
        QMessageBox::critical(nullptr, QObject::tr("NOT OK"),
                              QObject::tr("ajout failed.\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);
    ui->tab_client->setModel(c.afficher());
}
void MainWindow::on_pb_passer_commande_clicked()
{
    QSqlQuery query;
    QString ref = ui->le_ref->text();
    QString livreur = ui->le_livreur->text();
    QString client = ui->le_cin_client->text();
    QString adr = ui->le_adresse_commande->text();
    // int cin=ui->le_cin_client->text().toInt();

    int dd = ui->jj->text().toInt();
    int mm = ui->month_input_2->text().toInt();
    int yyyy = ui->year_input_2->text().toInt();
    float prix = ui->prix_input_2->text().toFloat();
    QString stock_id = ui->le_id_stock->text();

    QString status = ui->le_status->text();
    Commande Ctmp(ref, livreur, client, adr, dd, mm, yyyy, prix, stock_id, status);
    int pointsFidelite;
    bool test = Ctmp.ajouter();
    if (test)
    {
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("ajout effectué.\n"
                                             "Click Cancel to exit."),
                                 QMessageBox::Cancel);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("NOT OK"),
                              QObject::tr("ajout failed.\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);
    }
    int cin = client.toInt();
    if (c.existe(cin))
    {
        query.prepare("UPDATE CLIENT SET POINTSFIDELITE = POINTSFIDELITE + 10 WHERE CIN = :cin");
        query.bindValue(":cin", cin);
    }
}
void MainWindow::on_pb_supprimer_client_clicked()
{
    int cin = ui->le_cin_supp->text().toInt();
    Client c;
    if (c.existe(cin))
    {
        bool test = c.supprimer(cin);

        if (test)
        {
            QMessageBox::information(this, "Succès", "Suppression réussie.");
            ui->tab_client->setModel(c.afficher());
        }
        else
        {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression.");
        }
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Ce client n'existe pas.");
    }
}
void MainWindow::on_pb_chercher_client_clicked()
{
    int cin = ui->le_cin_3->text().toInt();

    bool test = c.existe(cin);
    if (test)
    {
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Ce Client existe!!.\n"
                                             "Click Cancel to exit."),
                                 QMessageBox::Cancel);
        QSqlQuery query;
        query.prepare("SELECT adresse , date_naissance, genre, mail, nom, numero, prenom FROM CLIENT WHERE cin = :cin");
        query.bindValue(":cin", cin);

        if (query.exec() && query.next())
        {
            QString adresse1 = query.value(0).toString();
            QString date_naissance1 = query.value(1).toString();
            QString genre1 = query.value(2).toString();
            QString mail1 = query.value(3).toString();
            QString nom1 = query.value(4).toString();
            int numero1 = query.value(5).toInt();
            QString prenom1 = query.value(6).toString();

            ui->le_cin_2->setText(QString::number(cin));
            ui->le_adresse_2->setText(adresse1);
            ui->le_date_2->setText(date_naissance1);
            ui->le_genre_2->setText(genre1);
            ui->le_mail_2->setText(mail1);
            ui->le_nom_2->setText(nom1);
            ui->le_numero_2->setText(QString::number(numero1));
            ui->le_prenom_2->setText(prenom1);
        }
    }
    else

        QMessageBox::critical(nullptr, QObject::tr("NOT OK"),
                              QObject::tr("Client n'existe pas.\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);
}
void MainWindow::on_pb_enregister_client_clicked()
{
    int cin = ui->le_cin_2->text().toInt();
    Client c;

    if (c.existe(cin))
    {
        QString adresse = ui->le_adresse_2->text();
        QString date_naissance = ui->le_date_2->text();
        QString genre = ui->le_genre_2->text();
        QString mail = ui->le_mail_2->text();
        QString nom = ui->le_nom_2->text();
        int numero = ui->le_numero_2->text().toInt();
        QString prenom = ui->le_prenom_2->text();

        QRegExp emailPattern("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,4}$");
        if (!emailPattern.exactMatch(mail))
        {
            QMessageBox::warning(this, "Erreur", "L'adresse e-mail n'est pas valide.");
            return; // Arrêtez le traitement si l'adresse e-mail est invalide.
        }

        if (genre != "Male" && genre != "Femme")
        {
            QMessageBox::warning(this, "Erreur", "Le genre doit être 'Male' ou 'Femme'.");
            return; // Arrêtez le traitement si le genre est invalide.
        }

        QRegExp datePattern("\\d{2}/\\d{2}/\\d{4}"); // Un motif régulier pour jj/mm/année
        if (!datePattern.exactMatch(date_naissance))
        {
            QMessageBox::warning(this, "Erreur", "La date de naissance doit être au format jj/mm/année.");
            return; // Arrêtez le traitement si la date est invalide.
        }

        bool test = c.modifier(adresse, cin, date_naissance, genre, mail, nom, numero, prenom);

        if (test)
        {
            QMessageBox::information(nullptr, QObject::tr("OK"),
                                     QObject::tr("modification effectuée.\n"
                                                 "Click Cancel to exit."),
                                     QMessageBox::Cancel);
        }
        else
        {
            QMessageBox::critical(nullptr, QObject::tr("NOT OK"),
                                  QObject::tr("modification échouée.\n"
                                              "Click Cancel to exit."),
                                  QMessageBox::Cancel);
        }
    }
    else
    {
        QMessageBox::warning(nullptr, QObject::tr("Client inexistant"),
                             QObject::tr("Le client avec le CIN spécifié n'existe pas !"), QMessageBox::Ok);
    }
    ui->tab_client->setModel(c.afficher());
}

void MainWindow::on_pb_trie_adresse_client_clicked()
{
    ui->tab_client->setModel(c.trierParAdresse());
}

void MainWindow::on_pb_trie_nom_client_clicked()
{
    ui->tab_client->setModel(c.trierParNom());
}

void MainWindow::on_pb_trie_date_client_clicked()
{
    ui->tab_client->setModel(c.trierParDateNaissance());
}

void MainWindow::on_pb_chercher_nom_client_clicked()
{
    QString nom = ui->le_chercher_nom->text();
    ui->tab_client->setModel(c.rechercherParNom(nom));
}

void MainWindow::on_pb_chercher_cin_client_clicked()
{
    int cin = ui->le_cin_chercher->text().toInt();
    ui->tab_client->setModel(c.rechercherParCIN(cin));
}

void MainWindow::on_pb_chercher_adresse_client_clicked()
{
    QString adresse = ui->le_adresse_chercher->text();
    ui->tab_client->setModel(c.rechercherParAdresse(adresse));
}

void MainWindow::clients_plus_fideles()
{
    int tauxPoints = 10;
    QSqlQuery query;
    query.prepare("SELECT CIN, NOM, MAIL FROM CLIENT WHERE POINTSFIDELITE > :POINTSFIDELITE");
    query.bindValue(":POINTSFIDELITE", tauxPoints);

    if (query.exec())
    {
        QString alertMessage;
        while (query.next())
        {
            QString cin = query.value(0).toString();
            QString nom = query.value(1).toString();
            QString mail = query.value(2).toString();

            // Retrieve the prix value for the client
            QSqlQuery prixQuery;
            prixQuery.prepare("SELECT PRIX FROM COMMANDE WHERE CLIENT = :CLIENT AND PRIX > 100");
            prixQuery.bindValue(":CLIENT", cin);

            if (prixQuery.exec() && prixQuery.next())
            {
                QString prix = prixQuery.value(0).toString();

                QString message = "Client ayant dépassé le taux :\nCIN : " + cin + "\nnom : " + nom + "\nE-mail : " + mail + "\nPrix : " + prix;
                alertMessage += message + "\n";
            }
        }

        if (!alertMessage.isEmpty())
        {
            ui->label_notification_client->setText("Taux de points de fidélité dépassé : ON A DES GAGNANTS !:\n" + alertMessage);
        }
        else
        {
            qDebug() << "Aucun client n'a dépassé le taux de points.";
        }
    }
    else
    {
        qDebug() << "Erreur lors de l'exécution de la requête : " << query.lastError().text();
    }
}

void MainWindow::on_PDF_2_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer PDF", "", "Fichiers PDF (*.pdf)");

    if (!filePath.isEmpty())
    {
        QPdfWriter pdfWriter(filePath);
        pdfWriter.setPageSizeMM(QSizeF(297, 210)); //

        QPainter painter(&pdfWriter);
        if (!painter.isActive())
        {
            return;
        }

        Client c;
        QSqlQueryModel *model = c.afficher();
        if (!model)
        {
            return;
        }

        const qreal leftMargin = 20;
        const qreal topMargin = 20;
        const qreal rightMargin = 20;
        const qreal bottomMargin = 20;

        pdfWriter.setPageMargins(QMarginsF(leftMargin, topMargin, rightMargin, bottomMargin));

        const QMarginsF pageMargins = pdfWriter.pageLayout().margins();
        const qreal pageLeftMargin = pageMargins.left();
        const qreal pageTopMargin = pageMargins.top();
        const qreal pageRightMargin = pageMargins.right();
        const qreal pageBottomMargin = pageMargins.bottom();

        const qreal tableWidth = pdfWriter.width() - pageLeftMargin - pageRightMargin;
        const qreal tableHeight = pdfWriter.height() - pageTopMargin - pageBottomMargin;

        const int rowCount = model->rowCount();
        const int columnCount = model->columnCount();
        const qreal lineHeight = 300;
        const qreal columnWidth = 2000;
        const QFont font("Arial", 10);

        qreal x = pageLeftMargin;
        qreal y = pageTopMargin;

        for (int column = 0; column < columnCount; ++column)
        {
            QRectF cellRect(x, y, columnWidth, lineHeight);
            painter.setFont(font);
            painter.drawText(cellRect, Qt::AlignCenter, model->headerData(column, Qt::Horizontal).toString());
            x += columnWidth;
        }

        y += lineHeight;

        for (int row = 0; row < rowCount; ++row)
        {
            x = pageLeftMargin;

            for (int column = 0; column < columnCount; ++column)
            {
                QRectF cellRect(x, y, columnWidth, lineHeight);
                painter.setFont(font);
                painter.drawText(cellRect, Qt::AlignLeft | Qt::AlignVCenter, model->data(model->index(row, column)).toString());
                x += columnWidth;
            }

            y += lineHeight;

            painter.setPen(QPen(Qt::black, 1));
            painter.drawLine(pageLeftMargin, y, pageLeftMargin + tableWidth, y);
        }

        painter.end();

        QMessageBox::information(this, "PDF généré", "Le fichier PDF a été généré avec succès.");
    }
}

void MainWindow::on_pb_statistique_genre_client_clicked()
{
    QMap<QString, int> genreCount;

    QSqlQuery query("SELECT genre, COUNT(*) FROM CLIENT GROUP BY genre");
    while (query.next())
    {
        QString genre = query.value(0).toString();
        int count = query.value(1).toInt();
        genreCount[genre] = count;
    }

    int total = genreCount["Male"] + genreCount["Femme"];

    QGraphicsScene *scene = new QGraphicsScene(this);
    QGraphicsView *view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);

    qreal startAngle = 0;
    for (const QString &genre : genreCount.keys())
    {
        qreal angle = 360.0 * genreCount[genre] / total;
        QColor sliceColor = (genre == "Male") ? QColor(255, 96, 0) : QColor(69, 69, 69);

        QGraphicsEllipseItem *slice = scene->addEllipse(-50, -50, 100, 100, QPen(Qt::black), QBrush(sliceColor));
        slice->setStartAngle(startAngle * 16);
        slice->setSpanAngle(angle * 16);

        qreal percentage = 100.0 * genreCount[genre] / total;
        QString percentageText = QString::number(percentage, 'f', 1) + "%";
        QGraphicsTextItem *label = scene->addText(percentageText, QFont("Lucida Bright", 8));
        label->setPos(70 * qCos(qDegreesToRadians(startAngle + angle / 2)),
                      70 * qSin(qDegreesToRadians(startAngle + angle / 2)));

        startAngle += angle;
    }

    scene->addRect(180, 20, 20, 20, QPen(Qt::black), QBrush(QColor(255, 96, 0)));
    scene->addText("Male", QFont("Lucida Bright", 10))->setPos(210, 25);

    scene->addRect(180, 50, 20, 20, QPen(Qt::black), QBrush(QColor(69, 69, 69)));
    scene->addText("Femme", QFont("Lucida Bright", 10))->setPos(210, 55);

    view->setRenderHint(QPainter::Antialiasing);
    view->setRenderHint(QPainter::TextAntialiasing);
    view->show();
}

void MainWindow::on_pb_statistique_client_fidele_clicked()
{
    QMap<QString, int> loyaltyPointsCount;

    QSqlQuery query("SELECT POINTSFIDELITE, COUNT(*) FROM CLIENT GROUP BY POINTSFIDELITE");
    while (query.next())
    {
        int loyaltyPoints = query.value(0).toInt();
        int count = query.value(1).toInt();

        QString category = (loyaltyPoints > 10) ? "Above 10" : "10 or Below";
        loyaltyPointsCount[category] += count;
    }

    int total = loyaltyPointsCount["Above 10"] + loyaltyPointsCount["10 or Below"];

    QGraphicsScene *scene = new QGraphicsScene(this);
    QGraphicsView *view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);

    qreal startAngle = 0;
    for (const QString &category : loyaltyPointsCount.keys())
    {
        qreal angle = 360.0 * loyaltyPointsCount[category] / total;
        QColor sliceColor = (category == "Above 10") ? QColor(255, 96, 0) : QColor(69, 69, 69);

        QGraphicsEllipseItem *slice = scene->addEllipse(-50, -50, 100, 100, QPen(Qt::black), QBrush(sliceColor));
        slice->setStartAngle(startAngle * 16);
        slice->setSpanAngle(angle * 16);

        qreal percentage = 100.0 * loyaltyPointsCount[category] / total;
        QString percentageText = QString::number(percentage, 'f', 1) + "%";
        QGraphicsTextItem *label = scene->addText(percentageText, QFont("Lucida Bright", 8));
        label->setPos(70 * qCos(qDegreesToRadians(startAngle + angle / 2)),
                      70 * qSin(qDegreesToRadians(startAngle + angle / 2)));

        startAngle += angle;
    }

    scene->addRect(180, 20, 20, 20, QPen(Qt::black), QBrush(QColor(255, 96, 0)));
    scene->addText("Above 10 Points", QFont("Lucida Bright", 10))->setPos(210, 25);

    scene->addRect(180, 50, 20, 20, QPen(Qt::black), QBrush(QColor(69, 69, 69)));
    scene->addText("10 Points or Below", QFont("Lucida Bright", 10))->setPos(210, 55);

    view->setRenderHint(QPainter::Antialiasing);
    view->setRenderHint(QPainter::TextAntialiasing);
    view->show();
}

void MainWindow::on_pb_statistique_adresse_client_clicked()
{
    QMap<QString, int> addressCount;

    QSqlQuery query("SELECT adresse, COUNT(*) FROM CLIENT GROUP BY adresse");
    while (query.next())
    {
        QString address = query.value(0).toString();
        int count = query.value(1).toInt();
        addressCount[address] = count;
    }

    int total = 0;
    for (const int count : addressCount.values())
    {
        total += count;
    }

    QGraphicsScene *scene = new QGraphicsScene(this);
    QGraphicsView *view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);

    QVector<QColor> customColors = {
        QColor(69, 69, 69),
        QColor(255, 96, 0),
        QColor(255, 165, 89),
        QColor(255, 230, 199)};

    qreal startAngle = 0;
    int colorIndex = 0;
    for (const QString &address : addressCount.keys())
    {
        qreal angle = 360.0 * addressCount[address] / total;
        QColor sliceColor = customColors.at(colorIndex);

        QGraphicsEllipseItem *slice = scene->addEllipse(-50, -50, 100, 100, QPen(Qt::black), QBrush(sliceColor));
        slice->setStartAngle(startAngle * 16);
        slice->setSpanAngle(angle * 16);

        qreal percentage = 100.0 * addressCount[address] / total;
        QString percentageText = QString::number(percentage, 'f', 1) + "%";
        QGraphicsTextItem *label = scene->addText(percentageText, QFont("Lucida Bright", 8));
        label->setPos(70 * qCos(qDegreesToRadians(startAngle + angle / 2)),
                      70 * qSin(qDegreesToRadians(startAngle + angle / 2)));

        startAngle += angle;
        colorIndex = (colorIndex + 1) % customColors.size();
    }

    int legendYPos = 20;
    colorIndex = 0;
    for (const QString &address : addressCount.keys())
    {
        QColor legendColor = customColors.at(colorIndex);
        scene->addRect(180, legendYPos, 20, 20, QPen(Qt::black), QBrush(legendColor));
        scene->addText(address, QFont("Lucida Bright", 10))->setPos(210, legendYPos + 5);
        legendYPos += 30;
        colorIndex = (colorIndex + 1) % customColors.size();
    }

    view->setRenderHint(QPainter::Antialiasing);
    view->setRenderHint(QPainter::TextAntialiasing);
    view->show();
}

/*void MainWindow::on_pushButton_valider_clicked()
{

    QSqlQueryModel model;
    QString at = "";
    QString at2 = "";
    QString rfid = ui->RFID->text();
    rfid = A.read_from_arduino();
    QSqlQuery query;
    model.setQuery("SELECT * FROM  reclamation  WHERE ( RFID  LIKE '" + ui->RFID->text() + "');");
    QString ref = model.data(model.index(0, 0)).toString();
    QString avis = model.data(model.index(0, 4)).toString();
    QString rfidd = model.data(model.index(0, 5)).toString();
    at = A.read_from_arduino();
    if (at == "" && at2 == "")
        at = ui->RFID->text();
    model.setQuery(query);

    qInfo() << "RFID= " << rfidd << " " << ref;

    if (rfidd == ui->RFID->text())
    {
        at = ref;
        at2 = avis;
        A.write_to_arduino("Ref:" + at + " " + "avis:" + at2);
        // A.write_to_arduino(at2);
    }
    else
    {
        QMessageBox::critical(this, "Erreur de connexion", "reclamation doesnt exicts");
    }
}*/

/*void MainWindow::on_pushButton_scan_clicked()
{
    QString at = A.read_from_arduino();

    at = at.simplified();
    at.replace(" ", "");

    if (at != "")
        ui->RFID->setText(at);
    qDebug() << "Erreur ";
}*/
//! ******************END MARIEM****************** !//

// dora

void MainWindow::on_pushButton_supprimer_clicked()
{
    int refToSupprimer = ui->comboBox_references->currentText().toInt();
    bool idExists = false;
    QSqlQuery query;
    query.prepare("SELECT ref FROM RECLAMATION WHERE ref = :ref");
    query.bindValue(":ref", refToSupprimer);

    if (query.exec() && query.next())
    {
        idExists = true;
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Erreur la référence dans la base de données.");
        return;
    }

    query.bindValue(":ref", refToSupprimer);
    if (idExists)
    {
        Reclamation R1;
        bool test = R1.supprimer_rec(refToSupprimer);
        if (test)
        {
            ui->tab_reclamation->setModel(R1.afficher_rec());
        }
        else
        {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression de la réclamation.");
        }
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "La référence n'existe pas dans la base de données.");
    }
}

void MainWindow::on_pushButton_ajouter_clicked()
{
    int ref = ui->la_ref->text().toInt();
    QDate datee = ui->la_date->date();
    bool retour_justifie = ui->retour_justifie->isChecked();
    bool retour_nonjustifie = ui->retour_nonjustifie->isChecked();
    QString avis = ui->l_avis->text();
    QString rfid = ui->rfid->text();

    Reclamation R(ref, datee, retour_justifie, retour_nonjustifie, avis, rfid);

    bool test = R.ajouter_rec();
    QMessageBox msgBox;
    if (test)
    {
        ui->tab_reclamation->setModel(R.afficher_rec());
        msgBox.setText("Ajout avec succes.");
    }
    else
    {
        msgBox.setText("Echec d'ajout.");
        msgBox.exec();
    }
    ui->tab_reclamation->setModel(rec.afficher_rec());
}

void MainWindow::on_pushButton_modifier_clicked()
{
    int ref_mod = ui->ref_mod->text().toInt();
    QDate datee_mod = ui->date_mod->date();
    bool retour_justifie_mod = ui->retour_justifie_mod->isChecked();
    bool retour_nonjustifie_mod = ui->retour_nonjustifie_mod->isChecked();
    QString avis_mod = ui->avis_mod->text();

    QString rfid = ui->rfid->text();
    Reclamation R(ref_mod, datee_mod, retour_justifie_mod, retour_nonjustifie_mod, avis_mod, rfid);
    bool test = R.modifier_rec(ref_mod);

    if (test)
    {
        QMessageBox::information(nullptr, QObject::tr("ok"),
                                 QObject::tr("update successful.\n"
                                             "update effectuer."),
                                 QMessageBox::Cancel);
        ui->tab_reclamation2->setModel(R.afficher_rec());
    }
    else

        QMessageBox::critical(nullptr, QObject::tr("nope"),
                              QObject::tr("connection failed.\n"
                                          "Click Cancel to exit."),
                              QMessageBox::Cancel);
}

void MainWindow::on_pushButton_rechercher_clicked()
{
    Reclamation R;
    QStringList referencesStringList;
    static bool referencesAdded = false;
    if (!referencesAdded)
    {
        QList<int> references = R.recupererToutesLesReferences();
        for (const int ref : references)
        {
            referencesStringList << QString::number(ref);
        }
        referencesAdded = true;
    }
    ui->rech_ref->addItems(referencesStringList);

    int ref = ui->rech_ref->currentText().toInt();
    ui->tab_reclamation2->setModel(R.rechercher_rec(ref));
}

/*void MainWindow::on_pushButton_rechercher_date_clicked()
{
Reclamation R;
QStringList DateStringList;
    static bool datesAdded = false;
    if (!datesAdded) {
           QList<int> dates = R.recupererToutesLesDates();
    for (const int datee : dates) {
        DateStringList << QString::number(datee);
    }
    datesAdded = true;
    }
    ui->rech_ref->addItems(DateStringList);


    QDate dateString = ui->rech_date->currentText();
    QDate datee = QDate::fromString(dateString, "yyyy-MM-dd");
    ui->tab_reclamation2->setModel(R.rechercher_rec_date(datee));
}*/

void MainWindow::on_pushButton_statistique_clicked()
{
    Reclamation R;
    float s;
    QString m;
    bool retour_nonjustifie = ui->retour_stat->text().toInt();
    s = R.statistique(retour_nonjustifie);
    m = "Statistique pour le retour non justifie " + QString::number(retour_nonjustifie) + ":";

    ui->la_msg->setText(m);
    ui->progressBar->setValue(s);
    QLineEdit *LineEdit1 = ui->retour_stat;
    LineEdit1->clear();
}

void MainWindow::on_pushButton_pdf_clicked()
{
    QString ach = ".pdf";
    QString pdfFilePath = "C:\\Users\\everp\\Downloads\\intg"   + ach;

    QPdfWriter pdf(pdfFilePath);

    // Créez un objet QFile pour écrire le fichier PDF
    QFile file(pdfFilePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Erreur", "Impossible d'initialiser le fichier PDF.");
        return;
    }

    QPainter painter(&pdf);

    int i = 4000;
    const int titleFontSize = 30;
    const int headerFontSize = 11;
    const int contentFontSize = 9; // Corrigé ici
    const int leftMargin = 200;
    const int column1 = leftMargin;
    const int column2 = 1200;
    const int column3 = 2400;
    const int column4 = 7000;
    const int column5 = 9000;

    painter.setPen(Qt::red);
    painter.setFont(QFont("Impact", titleFontSize));
    painter.drawText(2200, 1400, "Liste des réclamations");

    painter.setPen(Qt::black);
    painter.setFont(QFont("Impact", headerFontSize));
    painter.drawRect(0, 3000, 9600, 500);
    painter.drawText(leftMargin, 3300, "ref");
    painter.drawText(column2, 3300, "datee");
    painter.drawText(column3, 3300, "retour_justifie");
    painter.drawText(column4, 3300, "retour_nonjustifie");
    painter.drawText(column5, 3300, "avis");

    QSqlQuery query;
    query.prepare("SELECT * FROM RECLAMATION");
    if (query.exec())
    {
        painter.setFont(QFont("Arial", contentFontSize));
        while (query.next())
        {
            painter.drawText(leftMargin, i, query.value(0).toString());
            painter.drawText(column2, i, query.value(1).toString());
            painter.drawText(column3, i, query.value(2).toString());
            painter.drawText(column4, i, query.value(3).toString());
            painter.drawText(column5, i, query.value(4).toString());
            i += 500;
        }
    }

    painter.drawRect(0, 3000, 9600, i - 3000);

    int reponse = QMessageBox::question(this, "Générer PDF", "PDF enregistré. Voulez-vous afficher le PDF ?", QMessageBox::Yes | QMessageBox::No);
    if (reponse == QMessageBox::Yes)
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(pdfFilePath));
    }
}

void MainWindow::on_tri_ref_c_clicked()
{
    Reclamation R;
    ui->tab_reclamation2->setModel(R.tri_ref_c());
}

void MainWindow::on_tri_ref_dec_clicked()
{
    Reclamation R;
    ui->tab_reclamation2->setModel(R.tri_ref_dec());
}

void MainWindow::on_tri_dates_c_clicked()
{
    Reclamation R;
    ui->tab_reclamation2->setModel(R.tri_dates_c());
}

void MainWindow::on_tri_dates_dec_clicked()
{
    Reclamation R;
    ui->tab_reclamation2->setModel(R.tri_dates_dec());
}

void MainWindow::on_pushButton_historique_clicked()
{
    Reclamation *rec = new Reclamation();
    rec->afficherHistorique();
}

void MainWindow::on_pushButton_import_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Sélectionner un fichier", "", "Tous les fichiers (*.*)");
    if (!filePath.isEmpty())
    {
        QFileInfo fileInfo(filePath);
        QString fileName = fileInfo.fileName();
        if (fileName == "")
        {
            QMessageBox::information(nullptr, QObject::tr("Not OK"),
                                     QObject::tr("Valier sélectionner un fichier \n"
                                                 "Click Cancel to exit."),
                                     QMessageBox::Cancel);
        }
        else
        {
            QSqlQuery query;

            query.prepare("INSERT INTO FICHIER (NOM,CHEMIN) VALUES (:NOM, :CHEMIN)");
            query.bindValue(":NOM", fileName);
            query.bindValue(":CHEMIN", filePath);
            bool reqett = query.exec();
            if (reqett)
            {
                QMessageBox::information(nullptr, QObject::tr("OK"),
                                         QObject::tr("Ajout effectué \n"
                                                     "Click Cancel to exit."),
                                         QMessageBox::Cancel);
            }
            else
                QMessageBox::information(nullptr, QObject::tr("Not OK"),
                                         QObject::tr("Ajout non effectué \n"
                                                     "Click Cancel to exit."),
                                         QMessageBox::Cancel);
        }
    }
}

/*void MainWindow::on_pushButton_valider_clicked()
{
    QSqlQueryModel model;
      QString at="";
      QString at2="";
             QString rfid = ui->RFID->text();
              //rfid=A.read_from_arduino();
            QSqlQuery query;
            model.setQuery("SELECT * FROM  reclamation  WHERE ( RFID  LIKE '"+ui->RFID->text()+"');");
            QString ref=model.data(model.index(0, 0)).toString();
            QString avis=model.data(model.index(0, 4)).toString();
            QString rfidd=model.data(model.index(0, 5)).toString();
           // at=A.read_from_arduino();
         if (at==""&& at2=="")
                 at = ui->RFID->text();
                 model.setQuery(query);

          qInfo() << "RFID= "<<rfidd<<" "<<ref;

            if (rfidd==ui->RFID->text())
            {
                at=ref;
                at2=avis;
             //  A.write_to_arduino("Ref:"+at+" "+"avis:"+at2);
              // A.write_to_arduino(at2);

            } else {
                QMessageBox::critical(this, "Erreur de connexion", "reclamation doesnt exicts");
            }

}
*/

void MainWindow::on_pb_envoyer_client_clicked()
{
    QString mail = ui->le_destinataire->text();

    mailer::sendEmail(mail);
}

void MainWindow::on_refrech_sup_d_clicked()
{
    ui->comboBox_references->clear();
    Reclamation rec;
    QStringList referencesStringList;
    ui->tab_reclamation->setModel(rec.afficher_rec());

    for (const int ref : rec.recupererToutesLesReferences())
    {
        referencesStringList << QString::number(ref);
    }
    ui->comboBox_references->addItems(referencesStringList);
}
