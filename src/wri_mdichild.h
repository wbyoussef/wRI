#ifndef WRI_MDICHILD_H
#define WRI_MDICHILD_H
#include <QDialog>


class wRI_MdiChild : public QDialog
{
    Q_OBJECT
    enum MDI_TYPE  {WRI_PROJECT, WRI_OTHER};

public:
    wRI_MdiChild(QWidget *parent = 0);

    MDI_TYPE Type() const;
    void setType(const MDI_TYPE &Type);

    virtual bool load_file (QString filename) = 0;
    virtual bool save_file (QString filename) = 0;
    virtual bool save_file () = 0;

private :
    MDI_TYPE _Type;
};

#endif // WRI_MDICHILD_H
