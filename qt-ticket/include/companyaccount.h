#ifndef COMPANY_ACCOUNT_H
#define COMPANY_ACCOUNT_H
#include <QVariant>
#include <QDateTime>

class CompanyAccount {
public:
    CompanyAccount(qulonglong accountId, QDateTime time, qlonglong preBalance, qlonglong amount, qlonglong postBalance, qulonglong managerId, QString &managerName, qulonglong ticketId, QString &customer);

    inline qulonglong getAccountId() const { return _accountId; }
    inline QDateTime getTime() const { return _time; }
    inline qlonglong getPreBalance() const { return _preBalance; }
    inline qlonglong getAmount() const { return _amount; }
    inline qlonglong getPostBalance() const { return _postBalance; }
    inline qulonglong getManagerId() const { return _managerId; }
    inline QString getManagerName() const { return _managerName; }
    inline qulonglong getTicketId() const { return _ticketId; }
    inline QString getCustomer() const { return _customer; }

    static CompanyAccount *fromJson(QVariant var);

private:
    qulonglong _accountId;
    QDateTime _time;
    qlonglong _preBalance;
    qlonglong _amount;
    qlonglong _postBalance;
    qulonglong _managerId;
    QString _managerName;
    qulonglong _ticketId;
    QString _customer;
};


#endif

