#ifndef WRI_PROJECT_H
#define WRI_PROJECT_H

#include <QDialog>

namespace Ui {
class wRI_Project;
}

class wRI_Project : public QDialog
{
    Q_OBJECT

public:
    explicit wRI_Project(QWidget *parent = 0);
    ~wRI_Project();

private:
    Ui::wRI_Project *ui;
};

#endif // WRI_PROJECT_H
