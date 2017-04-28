#include "staticscustomer.h"
#include <QVariant>

StaticsCustomer::StaticsCustomer(qulonglong staticsId, QDateTime date, qlonglong regs, qlonglong amount, qlonglong netPaid)
    :_staticsId(staticsId),
     _date(date),
     _regs(regs),
     _amount(amount),
     _netPaid(netPaid)
{
}

StaticsCustomer *StaticsCustomer::fromJson(QVariant var)
{
    qulonglong staticsId = var.toMap()["id"].toULongLong();
    QDateTime date = QDateTime::fromMSecsSinceEpoch(var.toMap()["date"].toULongLong());
    qlonglong regs = var.toMap()["regs"].toLongLong();
    qlonglong amount = var.toMap()["amount"].toLongLong();
    qlonglong netPaid = var.toMap()["netPaid"].toLongLong();

    return new StaticsCustomer(staticsId, date, regs, amount, netPaid);
}


