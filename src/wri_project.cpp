#include "wri_project.h"
#include "ui_wri_project.h"

wRI_Project::wRI_Project(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::wRI_Project)
{
    ui->setupUi(this);
    this->ui->gridLayout->setMargin(0);
    this->ui->gridLayout_2->setMargin(0);
    this->ui->gridLayout->setSpacing(0);
    this->ui->gridLayout_2->setSpacing(0);

}

wRI_Project::~wRI_Project()
{
    delete ui;
}
