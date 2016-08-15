#include "wri_mdichild.h"

wRI_MdiChild::wRI_MdiChild(QWidget *parent )
{

}

wRI_MdiChild::MDI_TYPE wRI_MdiChild::Type() const
{
    return _Type;
}

void wRI_MdiChild::setType(const MDI_TYPE &Type)
{
    _Type = Type;
}

