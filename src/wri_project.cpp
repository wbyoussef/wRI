#include "wri_project.h"
#include "ui_wri_project.h"

wRI_Project::wRI_Project(QWidget *parent) :
    wRI_MdiChild(parent),
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

bool wRI_Project::load_file(QString filename)
{

}

bool wRI_Project::save_file(QString filename)
{

}
