#include "wri_project_doc.h"

wri_project_doc::wri_project_doc()
{

}

bool wri_project_doc::open(QString filename)
{

}

bool wri_project_doc::save(QString filename)
{
    //! save project name
    //! save rtl groups
    //! save project info
}

QString wri_project_doc::getSProjectName() const
{
    return sProjectName;
}

void wri_project_doc::setSProjectName(const QString &value)
{
    sProjectName = value;
}
QString wri_project_doc::getSDescription() const
{
    return sDescription;
}

void wri_project_doc::setSDescription(const QString &value)
{
    sDescription = value;
}

bool wri_project_doc::add_RtlGoup(QString name)
{

}

bool wri_project_doc::remove_RtlGroup(QString name)
{

}

wri_project_doc_RtlGroup *wri_project_doc::get_RtlGroup(QString name)
{

}
bool wri_project_doc::isChanged() const
{
    return Changed;
}

void wri_project_doc::setChanged(bool value)
{
    Changed = value;
}




