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
    app_settigns.p4port_pv = settings.value("app_settigns.p4port_pv").value<QString>();
    app_settigns.p4port_dev = settings.value("app_settigns.p4port_dev").value<QString>();
    app_settigns.default_path = settings.value("app_settigns.default_path").value<QString>();
    app_settigns.remote_cmd= settings.value("app_settigns.remote_cmd").value<QString>();
    app_settigns.PythonExec = settings.value("app_settigns.PythonExec").value<QString>();
    app_settigns.username = settings.value("app_settigns.username").value<QString>();
    int params_count =  settings.value("app_settigns.params_count").value<int>();
    for (int i = 0; i< params_count; i++)
    {
        app_settigns.params.append(settings.value(QString("app_settigns.params_") + QString::number(i)).value<QString>());
        app_settigns.values.append(settings.value(QString("app_settigns.value_") + QString::number(i)).value<QString>());

    }

    /// default values
    app_settigns.default_remote_host  = settings.value("app_settigns.default_remote_host").value<QString>();
    app_settigns.default_remote_path  = settings.value("app_settigns.default_remote_path").value<QString>();
    app_settigns.default_local_path   = settings.value("app_settigns.default_local_path").value<QString>();
    app_settigns.default_zebu_version = settings.value("app_settigns.default_zebu_version").value<QString>();
    app_settigns.default_file_conf    = settings.value("app_settigns.default_file_conf").value<QString>();

    // Update View
    this->ui->lineEdit_p4port_pv->setText( app_settigns.p4port_pv);
    this->ui->lineEdit_p4port_dev->setText( app_settigns.p4port_dev);
    this->ui->lineEdit_default_path->setText( app_settigns.default_path);
    this->ui->lineEdit_remotecmd->setText( app_settigns.remote_cmd);
    this->ui->lineEdit_PythonExec->setText( app_settigns.PythonExec);
    this->ui->lineEdit_username->setText( app_settigns.username);

    /// default values
    this->ui->lineEdit_default_remote_host->setText( app_settigns.default_remote_host);
    this->ui->lineEdit_default_remote_pth->setText( app_settigns.default_remote_path);
    this->ui->lineEdit_default_local_path->setText( app_settigns.default_local_path);
    this->ui->lineEdit_default_zebu_version->setText( app_settigns.default_zebu_version);
    this->ui->lineEdit_default_file_conf->setText( app_settigns.default_file_conf);




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
    app_settigns.p4port_pv = this->ui->lineEdit_p4port_pv->text() ;
    app_settigns.p4port_dev= this->ui->lineEdit_p4port_dev->text() ;
    app_settigns.default_path= this->ui->lineEdit_default_path->text() ;
    app_settigns.remote_cmd= this->ui->lineEdit_remotecmd->text() ;
    app_settigns.PythonExec = this->ui->lineEdit_PythonExec->text() ;
    app_settigns.username= this->ui->lineEdit_username->text() ;

    app_settigns.params.clear();
    app_settigns.values.clear();
    for (int i = 0; i< this->ui->tableWidget_params->rowCount(); i++)
    {
        QTableWidgetItem* item= this->ui->tableWidget_params->item(i,0);
        app_settigns.params.append(item->text());
        item= this->ui->tableWidget_params->item(i,1);
        app_settigns.values.append(item->text());
    }


    /// default values
    app_settigns.default_remote_host  = this->ui->lineEdit_default_remote_host->text( );
    app_settigns.default_remote_path  = this->ui->lineEdit_default_remote_pth->text( );
    app_settigns.default_local_path   = this->ui->lineEdit_default_local_path->text( );
    app_settigns.default_zebu_version = this->ui->lineEdit_default_zebu_version->text( );
    app_settigns.default_file_conf    = this->ui->lineEdit_default_file_conf->text( );




    QSettings settings("wDev", "wRTLDesk");
    // settings.setValue("pos", pos());
    settings.setValue("app_settigns.p4port_pv", app_settigns.p4port_pv);
    settings.setValue("app_settigns.p4port_dev", app_settigns.p4port_dev );
    settings.setValue("app_settigns.default_path", app_settigns.default_path );
    settings.setValue("app_settigns.remote_cmd", app_settigns.remote_cmd);
    settings.setValue("app_settigns.PythonExec", app_settigns.PythonExec);
    settings.setValue("app_settigns.username",app_settigns.username );

    settings.setValue("app_settigns.params_count",app_settigns.params.count());

    for (int i =0; i< app_settigns.params.count();i++)
    {
        settings.setValue(QString("app_settigns.params_") + QString::number(i),app_settigns.params.at(i) );
        settings.setValue(QString("app_settigns.value_")  + QString::number(i),app_settigns.values.at(i) );
    }

    /// default values
    settings.setValue("app_settigns.default_remote_host",app_settigns.default_remote_host);
    settings.setValue("app_settigns.default_remote_path",app_settigns.default_remote_path);
    settings.setValue("app_settigns.default_local_path",app_settigns.default_local_path);
    settings.setValue("app_settigns.default_zebu_version",app_settigns.default_zebu_version);
    settings.setValue("app_settigns.default_file_conf",app_settigns.default_file_conf);

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


QString wRI_Desk_settings::get_shell_script()
{
    QString tmp_return ="";
    tmp_return += "export REMOTECMD=\"" + this->app_settigns.remote_cmd + " \" ;  \n";
    tmp_return += "export REMOTESYN=\"" + this->app_settigns.remote_cmd + " \" ;  \n";
    tmp_return += "source /auto/qa/common.bash \n" ;
    tmp_return += "source /auto/import/sun/sge/default/common/settings.sh \n";

    return tmp_return;
}

void wRI_Desk_settings::on_pushButton_clicked()
{
    this->close();
}

void wRI_Desk_settings::on_toolButton_3_clicked()
{
    FILE *fp;
    int status;
    char tc_name[1000];

    /* Open the command for reading. */
    QString command_to_run = "cd /tmp ; export CVSROOT=";
    command_to_run += this->app_settigns.p4port_pv.trimmed() + "; ";
    command_to_run += "cvs -d " +
                      this->app_settigns.p4port_pv.trimmed() +
                       "  co -l -d qa_desk qa/xl/eve ;";

    command_to_run += "cd qa_desk; cvs -n up -d 2>&1";
    this->ui->listWidget_tests_list->clear();
    fp = popen( command_to_run.toStdString().c_str(), "r");

    if (fp == NULL) {
      printf("Failed to run command\n" );
      exit;
    }

    /* Read the output a line at a time - output it. */
    while (fgets(tc_name, sizeof(tc_name)-1, fp) != NULL) {
      QString tests_raw =  QString (tc_name);
      if (tests_raw.contains("cvs server: New directory"))
      {
          // get test name
          tests_raw.remove("\n");
          tests_raw.remove("cvs server: New directory `");
          tests_raw.remove("' -- ignored");
          this->ui->listWidget_tests_list->addItem(tests_raw);
      }
    }


    /* close */
    pclose(fp);

}
