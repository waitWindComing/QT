#include "companyaccount.h"

CompanyAccount::CompanyAccount(qulonglong accountId, QDateTime time, qlonglong preBalance, qlonglong amount, qlonglong postBalance, qulonglong managerId, QString &managerName, qulonglong ticketId, QString &customer)
    :_accountId(accountId),
    _time(time),
    _preBalance(preBalance),
    _amount(amount),
    _postBalance(postBalance),
    _managerId(managerId),
    _managerName(managerName),
    _ticketId(ticketId),
    _customer(customer)
{
}


CompanyAccount *CompanyAccount::fromJson(QVariant var)
{
    qulonglong accountId = var.toMap()["id"].toULongLong();
    QDateTime time = QDateTime::fromMSecsSinceEpoch(var.toMap()["time"].toULongLong());
    qlonglong preBalance = var.toMap()["preBalance"].toLongLong();
    qlonglong amount = var.toMap()["amount"].toLongLong();
    qlonglong postBalance = var.toMap()["postBalance"].toLongLong();
    qulonglong managerId = 0L;
    QString managerName;
    QVariant managerVar = var.toMap()["manager"];
    if (!managerVar.isNull()) {
        managerId = managerVar.toMap()["id"].toULongLong();
        managerName = managerVar.toMap()["name"].toString();
    }
    qulonglong ticketId = 0L;
    QString customer;
    QVariant ticketUseVar = var.toMap()["ticketUse"];
    if (!ticketUseVar.isNull()) {
        ticketId = ticketUseVar.toMap()["ticket"].toMap()["id"].toULongLong();
        customer = ticketUseVar.toMap()["usedBy"].toMap()["name"].toString();
    }

    return new CompanyAccount(accountId, time, preBalance, amount, postBalance, managerId, managerName, ticketId, customer);
}


