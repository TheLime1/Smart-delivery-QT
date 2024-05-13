#include "gestion_produitt.h"
#include "ui_gestion_produitt.h"

gestion_produitt::gestion_produitt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::gestion_produitt)
{
    ui->setupUi(this);
}

gestion_produitt::~gestion_produitt()
{
    delete ui;
}
