#include "product.h"

Product::Product(qulonglong productId, QString &name, QString &unit, QString &contentType)
    :_productId(productId),
     _name(name),
     _unit(unit),
     _contentType(contentType)
{
}

Product Product::fromJson(QVariant var)
{
    qulonglong productId = var.toMap()["id"].toULongLong();
    QString name = var.toMap()["name"].toString();
    QString unit = var.toMap()["unit"].toString();
    QString contentType = var.toMap()["contentType"].toString();
    return Product(productId, name, unit, contentType);
}

