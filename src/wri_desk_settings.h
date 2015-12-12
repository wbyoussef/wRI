#ifndef WRI_DESK_SETTINGS_H
#define WRI_DESK_SETTINGS_H

#include <QDialog>

namespace Ui {
    class wRI_Desk_settings;
}
class wRI_Desk_settings_struct
{
public:
    QString username;
    QString default_path;
    QString p4port_pv;
    QString p4port_dev;
    QString remote_cmd;
    QString PythonExec;
    QStringList params;
    QStringList values;

    /// Default Values
    QString default_remote_host;
    QString default_remote_path;
    QString default_local_path;
    QString default_zebu_version;
    QString default_file_conf;

};


class wRI_Desk_settings : public QDialog
{
    Q_OBJECT

public:
    explicit wRI_Desk_settings(QWidget *parent = 0);
    ~wRI_Desk_settings();

    wRI_Desk_settings_struct app_settigns;
    QString get_shell_script();
private slots:
    void on_toolButton_3_clicked();
    void on_pushButton_clicked();
    void on_toolButton_4_clicked();

    void on_toolButton_add_param_clicked();

    void on_toolButton_2_clicked();



private:
    Ui::wRI_Desk_settings *ui;
};

#endif // WRI_DESK_SETTINGS_H
