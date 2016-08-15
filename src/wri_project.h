#ifndef WRI_PROJECT_H
#define WRI_PROJECT_H

#include <QDialog>

#include "wri_mdichild.h"

namespace Ui {
class wRI_Project;
}

class wri_project_doc;

class wRI_Project : public wRI_MdiChild
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
    bool save_file ();

    QString FileName() const;
    void setFileName(const QString &FileName);

private:
    Ui::wRI_Project *ui;
    //! Document
    wri_project_doc* _Document;
    QString _FileName;
};

#endif // WRI_PROJECT_H
