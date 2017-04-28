#include "companyproduct.h"
#include <QVariant>

CompanyProduct::CompanyProduct(qulonglong companyProductId, Company &company, Product &product, CompanyProductPrice &currentPrice)
    :_companyProductId(companyProductId),
     _company(company),
     _product(product),
     _currentPrice(currentPrice)
{
}

CompanyProduct CompanyProduct::fromJson(QVariant var)
{
    qulonglong companyProductId = var.toMap()["id"].toULongLong();
    Company company = Company::fromJson(var.toMap()["company"]);
    Product product = Product::fromJson(var.toMap()["product"]);
    CompanyProductPrice currentPrice = CompanyProductPrice::fromJson(var.toMap()["currentPrice"]);
    return CompanyProduct(companyProductId, company, product, currentPrice);
}

