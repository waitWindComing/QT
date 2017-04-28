#ifndef STATICS_CUSTOMER_H
#define STATICS_CUSTOMER_H
#include <QDateTime>

class StaticsCustomer {
public:
    StaticsCustomer(qulonglong staticsId, QDateTime date, qlonglong regs, qlonglong amount, qlonglong netPaid);

    inline qulonglong getStaticsId() const { return _staticsId; }
    inline QDateTime getDate() const { return _date; }
    inline qlonglong getRegs() const { return _regs; }
    inline qlonglong getAmount() const { return _amount; }
    inline qlonglong getNetPaid() const { return _netPaid; }

    static StaticsCustomer *fromJson(QVariant var);

private:
    qulonglong _staticsId;
    QDateTime _date;
    qlonglong _regs;
    qlonglong _amount;
    qlonglong _netPaid;
};

#endif

