#include "writ_project_doc.h"

writ_project_doc::writ_project_doc()
{

}
QString writ_project_doc::getSProjectName() const
{
    return sProjectName;
}

void writ_project_doc::setSProjectName(const QString &value)
{
    sProjectName = value;
}
QString writ_project_doc::getSDescription() const
{
    return sDescription;
}

void writ_project_doc::setSDescription(const QString &value)
{
    sDescription = value;
}



