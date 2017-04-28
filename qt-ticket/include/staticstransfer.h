#ifndef STATICS_TRANSFER_H
#define STATICS_TRANSFER_H
#include <QDateTime>

class StaticsTransfer {
public:
    StaticsTransfer(qulonglong staticsId, QDateTime date, qlonglong regs, qlonglong count);
    ~StaticsTransfer();

    inline qulonglong getStaticsId() const { return _staticsId; }
    inline QDateTime getDate() const { return _date; }
    inline qlonglong getRegs() const { return _regs; }
    inline qlonglong getCount() const { return _count; }

    static StaticsTransfer *fromJson(QVariant var);

private:
    qulonglong _staticsId;
    QDateTime _date;
    qlonglong _regs;
    qlonglong _count;
};

#endif

