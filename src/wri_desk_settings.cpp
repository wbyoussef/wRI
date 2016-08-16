#include "wri_desk_settings.h"
#include "ui_wri_desk_settings.h"
#include <QSettings>
#include <iostream>
#include <stdio.h>

wRI_Desk_settings::wRI_Desk_settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::wRI_Desk_settings)
{
    ui->setupUi(this);
    QSettings settings("wDev", "wRTLDesk");
    int params_count =  settings.value("app_settigns.params_count").value<int>();
    for (int i = 0; i< params_count; i++)
    {
        app_settigns.params.append(settings.value(QString("app_settigns.params_") + QString::number(i)).value<QString>());
        app_settigns.values.append(settings.value(QString("app_settigns.value_") + QString::number(i)).value<QString>());

    }

    /// default values
    app_settigns.default_zebu_version = settings.value("app_settigns.default_zebu_version").value<QString>();

    // Update View

    /// default values
    this->ui->lineEdit_default_zebu_version->setText( app_settigns.default_zebu_version);




    for (int i = 0; i< app_settigns.params.count(); i++)
    {
        QTableWidgetItem* item0 = new QTableWidgetItem;
        item0->setText(app_settigns.params.at(i));
        QTableWidgetItem* item1 = new QTableWidgetItem;
        item1->setText(app_settigns.values.at(i));

       int row = this->ui-> tableWidget_params->rowCount();
       this->ui->tableWidget_params->insertRow(row);
       this->ui->tableWidget_params->setItem(row, 0, item0);
       this->ui->tableWidget_params->setItem(row, 1, item1);
    }

}

wRI_Desk_settings::~wRI_Desk_settings()
{
    delete ui;
}

void wRI_Desk_settings::on_toolButton_4_clicked()
{
    // update params

    app_settigns.params.clear();
    app_settigns.values.clear();
    for (int i = 0; i< this->ui->tableWidget_params->rowCount(); i++)
    {
        QTableWidgetItem* item= this->ui->tableWidget_params->item(i,0);
        app_settigns.params.append(item->text());
        item= this->ui->tableWidget_params->item(i,1);
        app_settigns.values.append(item->text());
    }


    app_settigns.default_zebu_version = this->ui->lineEdit_default_zebu_version->text( );




    QSettings settings("wDev", "wRTLDesk");
    // settings.setValue("pos", pos());

    settings.setValue("app_settigns.params_count",app_settigns.params.count());

    for (int i =0; i< app_settigns.params.count();i++)
    {
        settings.setValue(QString("app_settigns.params_") + QString::number(i),app_settigns.params.at(i) );
        settings.setValue(QString("app_settigns.value_")  + QString::number(i),app_settigns.values.at(i) );
    }

    /// default values
    settings.setValue("app_settigns.default_zebu_version",app_settigns.default_zebu_version);

    this->close();
}

void wRI_Desk_settings::on_toolButton_add_param_clicked()
{

    QTableWidgetItem* item0 = new QTableWidgetItem;
    QTableWidgetItem* item1 = new QTableWidgetItem;

   int row = this->ui-> tableWidget_params->rowCount();
   this->ui->tableWidget_params->insertRow(row);
   this->ui->tableWidget_params->setItem(row, 0, item0);
   this->ui->tableWidget_params->setItem(row, 1, item1);


}

void wRI_Desk_settings::on_toolButton_2_clicked()
{


    int x = ((this->ui->tableWidget_params->selectedItems()).first())->row();
    this->ui->tableWidget_params->removeRow(x);

}



void wRI_Desk_settings::on_pushButton_clicked()
{
    this->close();
}


