#ifndef WRIT_PROJECT_DOC_RTLGROUP_H
#define WRIT_PROJECT_DOC_RTLGROUP_H

#include <QString>

class writ_project_doc_RtlGroup
{
public :
    writ_project_doc_RtlGroup();
    QString getSGroupName() const;
    void setSGroupName(const QString &value);

private :
    QString sGroupName;
};

#endif // WRIT_PROJECT_DOC_RTLGROUP_H
