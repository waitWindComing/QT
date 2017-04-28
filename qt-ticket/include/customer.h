#ifndef CUSTOMER_H
#define CUSTOMER_H
#include <QWidget>
#include <QVariant>

class Customer {
public:
    Customer(qulonglong customerId, QString &customerName);

    inline qulonglong getCustomerId() const { return _customerId; }
    inline QString getName() const { return _customerName; }

    static Customer *fromJson(QVariant var);

private:
    qulonglong _customerId;
    QString _customerName;
};

#endif

