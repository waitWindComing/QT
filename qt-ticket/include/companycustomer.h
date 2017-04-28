#ifndef COMPANY_CUSTOMER_H
#define COMPANY_CUSTOMER_H
#include <QVariant>
#include <QDateTime>

class Customer;
class CompanyCustomer {
public:
    CompanyCustomer(qulonglong companyCustomerId, Customer *customer, QDateTime &regWhen, qlonglong amount, qlonglong times, int discount, qlonglong balance);
    ~CompanyCustomer();

    inline qulonglong getCompanyCustomerId() const { return _companyCustomerId; }
    inline Customer *getCustomer() const { return _customer; }
    inline QDateTime getRegWhen() const { return _regWhen; }
    inline qlonglong getAmount() const { return _amount; }
    inline qlonglong getTimes() const { return _times; }
    inline int getDiscount() const { return _discount; }
    inline qlonglong getBalance() const { return _balance; }

    static CompanyCustomer *fromJson(QVariant var);

private:
    qulonglong _companyCustomerId;
    Customer *_customer;
    QDateTime _regWhen;
    qlonglong _amount;
    qlonglong _times;
    int _discount;
    qlonglong _balance;
};

#endif

