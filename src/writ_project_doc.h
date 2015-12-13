#ifndef WRIT_PROJECT_DOC_H
#define WRIT_PROJECT_DOC_H

#include <QString>
#include <QVector>
#include "writ_project_doc_info.h"
#include "writ_project_doc_rtlgroup.h"
//#include "writ_project_doc_RtlGroup.h"

class writ_project_doc
{
public:
    writ_project_doc();

    QString getSProjectName() const;
    void setSProjectName(const QString &value);

    QString getSDescription() const;
    void setSDescription(const QString &value);

private :
    //!Project name
    QString    sProjectName;
    //! Project description
    QString    sDescription;
    //! Rtl groups
    QVector <writ_project_doc_RtlGroup>  RtlGroups ;
};

#endif // WRIT_PROJECT_DOC_H
