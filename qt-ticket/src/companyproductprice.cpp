#include "companyproductprice.h"

CompanyProductPrice::CompanyProductPrice(qulonglong companyProductPriceId, qlonglong price)
    :_companyProductPriceId(companyProductPriceId),
     _price(price)
{
}

CompanyProductPrice CompanyProductPrice::fromJson(QVariant var)
{
    qulonglong companyProductPriceId = var.toMap()["id"].toULongLong();
    qlonglong price = var.toMap()["price"].toLongLong();
    return CompanyProductPrice(companyProductPriceId, price);
}

