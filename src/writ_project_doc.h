#ifndef WRIT_PROJECT_DOC_H
#define WRIT_PROJECT_DOC_H

#include <QString>
#include <writ_project_doc_info.h>

class writ_project_doc_RtlGroup
{

};

class writ_project_doc
{
public:
    writ_project_doc();

    QString getSProjectName() const;
    void setSProjectName(const QString &value);

    QString getSDescription() const;
    void setSDescription(const QString &value);

    writ_project_doc_RtlGroup getRtlGroup() const;
    void setRtlGroup(const writ_project_doc_RtlGroup &value);

private :
    QString    sProjectName;
    QString    sDescription;
    writ_project_doc_RtlGroup  RtlGroup ;
};

#endif // WRIT_PROJECT_DOC_H
