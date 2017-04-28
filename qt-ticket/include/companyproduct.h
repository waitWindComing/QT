#ifndef COMPANY_PRODUCT_H
#define COMPANY_PRODUCT_H
#include <QWidget>
#include <QVariant>

#include "company.h"
#include "product.h"
#include "companyproductprice.h"

class CompanyProduct {
public:
    CompanyProduct(qulonglong companyProductId, Company &company, Product &product, CompanyProductPrice &currentPrice);
    inline qulonglong getCompanyProductId() const { return _companyProductId; }
    inline Company getCompany() const { return _company; }
    inline Product getProduct() const { return _product; }
    inline CompanyProductPrice getCurrentPrice() const { return _currentPrice; }

    static CompanyProduct fromJson(QVariant var);

private:
    qulonglong _companyProductId;
    Company _company;
    Product _product;
    CompanyProductPrice _currentPrice;
};

inline bool operator <(const CompanyProduct &p1, const CompanyProduct &p2)
{
    return p1.getCompanyProductId() < p2.getCompanyProductId();
}

inline bool operator==(const CompanyProduct &p1, const CompanyProduct &p2)
{
    return p1.getCompanyProductId() == p2.getCompanyProductId();
}

inline uint qHash(const CompanyProduct &key) 
{
    return key.getCompanyProductId(); 
}

#endif

