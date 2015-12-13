#include "writ_project_doc_rtlgroup.h"

writ_project_doc_RtlGroup::writ_project_doc_RtlGroup()
{

}


QString writ_project_doc_RtlGroup::getSGroupName() const
{
    return sGroupName;
}

void writ_project_doc_RtlGroup::setSGroupName(const QString &value)
{
    sGroupName = value;
}


QString writ_project_doc_RtlGroup_file::getFilename() const
{
    return filename;
}

void writ_project_doc_RtlGroup_file::setFilename(const QString &value)
{
    filename = value;
}
