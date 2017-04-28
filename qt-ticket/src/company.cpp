#include "company.h"
#include <qDebug>

Company::Company(qulonglong companyId, QString &companyName, double &latitude, double &longitude)
    :_companyId(companyId),
     _companyName(companyName),
     _latitude(latitude),
     _longitude(longitude)
{
}

bool Company::operator==(const Company & other) const
{
    if (_companyId == other.getCompanyId())
        return true;
    else
        return false;
}

bool Company::operator!=(const Company & other) const
{
    if (_companyId != other.getCompanyId())
        return true;
    else
        return false;
}

Company Company::fromJson(QVariant var)
{
    qulonglong companyId = var.toMap()["id"].toULongLong();
    QString companyName = var.toMap()["name"].toString();
    double latitude = var.toMap()["latitude"].toDouble();
    double longitude = var.toMap()["longitude"].toDouble();
    return Company(companyId, companyName, latitude, longitude);
}


