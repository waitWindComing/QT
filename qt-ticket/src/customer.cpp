#include "customer.h"

Customer::Customer(qulonglong customerId, QString &customerName)
    :_customerId(customerId),
     _customerName(customerName)
{
}

Customer *Customer::fromJson(QVariant var)
{
    qulonglong customerId = var.toMap()["id"].toULongLong();
    QString customerName = var.toMap()["name"].toString();
    return new Customer(customerId, customerName);
}

