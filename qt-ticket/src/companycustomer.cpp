#include "companycustomer.h"
#include "customer.h"

CompanyCustomer::CompanyCustomer(qulonglong companyCustomerId, Customer *customer, QDateTime &regWhen, qlonglong amount, qlonglong times, int discount, qlonglong balance) 
    : _companyCustomerId(companyCustomerId),
      _customer(customer),
      _regWhen(regWhen),
      _amount(amount),
      _times(times),
      _discount(discount),
      _balance(balance)
{
}

CompanyCustomer::~CompanyCustomer() 
{
    delete _customer;
}

CompanyCustomer *CompanyCustomer::fromJson(QVariant var)
{
    qulonglong companyCustomerId = var.toMap()["id"].toULongLong();
    Customer *customer = Customer::fromJson(var.toMap()["customer"]);
    QDateTime regWhen = QDateTime::fromMSecsSinceEpoch(var.toMap()["regWhen"].toULongLong());
    qlonglong amount = var.toMap()["amount"].toLongLong();
    qlonglong times = var.toMap()["times"].toLongLong();
    int discount = var.toMap()["discount"].toInt();
    qlonglong balance = var.toMap()["balance"].toLongLong();
    return new CompanyCustomer(companyCustomerId, customer, regWhen, amount, times, discount, balance);
}

