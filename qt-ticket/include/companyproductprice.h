#ifndef COMPANY_PRODUCT_PRICE_H
#define COMPANY_PRODUCT_PRICE_H
#include <QWidget>
#include <QVariant>

class CompanyProductPrice {
public:
    CompanyProductPrice(qulonglong companyProductPriceId, qlonglong price);

    inline qulonglong getCompanyProductPriceId() const { return _companyProductPriceId; }
    inline qlonglong getPrice() const { return _price; }

    static CompanyProductPrice fromJson(QVariant var);

private:
    qulonglong _companyProductPriceId;
    qlonglong _price;
};
#endif

