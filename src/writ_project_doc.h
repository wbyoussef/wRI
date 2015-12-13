#ifndef WRIT_PROJECT_DOC_H
#define WRIT_PROJECT_DOC_H

#include <QString>


class cRtlGroup
{
public :
    cRtlGroup();
    QString getSGroupName() const;
    void setSGroupName(const QString &value);

private :
    QString sGroupName;
};

class writ_project_doc
{
public:
    writ_project_doc();

    QString getSProjectName() const;
    void setSProjectName(const QString &value);

    QString getSDescription() const;
    void setSDescription(const QString &value);

    cRtlGroup getRtlGroup() const;
    void setRtlGroup(const cRtlGroup &value);

private :
    QString    sProjectName;
    QString    sDescription;
    cRtlGroup  RtlGroup ;
};

#endif // WRIT_PROJECT_DOC_H
