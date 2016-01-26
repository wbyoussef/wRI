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

    /*!
     *  \brief Open project file
     *     @param [in] File name to load
     *     @return True if file loaded successfully
    */
    bool load_file (QString filename);

    /*!
     *  \brief Open project file
     *     @param [in] File name to load
     *     @return True if file loaded successfully
    */
    bool save_file (QString filename);

private:
    Ui::wRI_Project *ui;
};

#endif // WRI_PROJECT_H
