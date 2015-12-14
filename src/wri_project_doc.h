#ifndef wri_project_DOC_H
#define wri_project_DOC_H

#include <QString>
#include <QVector>
#include "wri_project_doc_info.h"
// #include "wri_project_doc_rtlgroup.h"

class wri_project_doc_RtlGroup;

class wri_project_doc
{
public:
    enum file_type  {FILE_VLOG,FILE_SVLOG, FILE_VHDL,FILE_SYSTEMC,FILE_XML,FILE_TCL};

    wri_project_doc();

    //! load project
    bool open (QString filename) ;
    bool save (QString filename) ;

    QString getSProjectName() const;
    void setSProjectName(const QString &value);

    QString getSDescription() const;
    void setSDescription(const QString &value);

    //! Add an RTL group
    bool add_RtlGoup (QString name);
    //! remove an RTL Group
    bool remove_RtlGroup (QString name);
    //! Get An RTL Group
    wri_project_doc_RtlGroup* get_RtlGroup (QString name);


    bool isChanged() const;
    void setChanged(bool value);

private :
    //!Project name
    QString    sProjectName;
    //! Project description
    QString    sDescription;
    //! Rtl groups
    QVector <wri_project_doc_RtlGroup*>  RtlGroups ;
    bool Changed;
};

#endif // wri_project_DOC_H
