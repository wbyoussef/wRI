#include "wri_project.h"
#include "ui_wri_project.h"

wRI_Project::wRI_Project(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::wRI_Project)
{
    ui->setupUi(this);
}

wRI_Project::~wRI_Project()
{
    delete ui;
}
