#ifndef wri_project_DOC_RTLGROUP_H
#define wri_project_DOC_RTLGROUP_H

#include <QString>
#include <QVector>

#include "wri_project_doc.h"
class wri_project_doc;

class wri_project_doc_RtlGroup_file
{
public :
    wri_project_doc_RtlGroup_file();
    QString getFilename() const;
    void setFilename(const QString &value);

    wri_project_doc::file_type getFile_type() const;
    void setFile_type(const wri_project_doc::file_type &value);

private :
    QString filename;
    wri_project_doc::file_type file_type;

};

class wri_project_doc_RtlGroup
{
public :
    wri_project_doc_RtlGroup();
    QString getSGroupName() const;
    void setSGroupName(const QString &value);

private :
    QString sGroupName;
    QVector <wri_project_doc_RtlGroup_file*> files;
};

#endif // wri_project_DOC_RTLGROUP_H
