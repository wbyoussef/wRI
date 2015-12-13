#ifndef WRIT_PROJECT_DOC_H
#define WRIT_PROJECT_DOC_H

#include <QString>


class cRtlGroup
{
public :
    cRtlGroup();
private :
    QString sGroupName;
};

class writ_project_doc
{
public:
    writ_project_doc();

private :
    QString    sProjectName;
    QString    sDescription;
    cRtlGroup  RtlGroup ;
};

#endif // WRIT_PROJECT_DOC_H
