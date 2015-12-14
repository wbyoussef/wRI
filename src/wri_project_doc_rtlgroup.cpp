#include "wri_project_doc_rtlgroup.h"

wri_project_doc_RtlGroup::wri_project_doc_RtlGroup()
{

}


QString wri_project_doc_RtlGroup::getSGroupName() const
{
    return sGroupName;
}

void wri_project_doc_RtlGroup::setSGroupName(const QString &value)
{
    sGroupName = value;
}


QString wri_project_doc_RtlGroup_file::getFilename() const
{
    return filename;
}

void wri_project_doc_RtlGroup_file::setFilename(const QString &value)
{
    filename = value;
}
WRI_PROJECT::file_type wri_project_doc_RtlGroup_file::getFile_type() const
{
    return file_type;
}

void wri_project_doc_RtlGroup_file::setFile_type(const WRI_PROJECT::file_type &value)
{
    file_type = value;
}

