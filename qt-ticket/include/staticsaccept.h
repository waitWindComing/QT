#ifndef STATICS_ACCEPT_H
#define STATICS_ACCEPT_H
#include "company.h"
#include <QDateTime>

class UseRule;
class StaticsAccept {
public:
    StaticsAccept(qulonglong staticsId, QDateTime date, Company usedIn, Company issuedBy, UseRule *useRule, qlonglong save, qlonglong count);
    ~StaticsAccept();

    inline qulonglong getStaticsId() const { return _staticsId; }
    inline QDateTime getDate() const { return _date; }
    inline Company getUsedIn() const { return _usedIn; }
    inline Company getIssuedBy() const { return _issuedBy; }
    inline UseRule *getUseRule() const { return _useRule; }
    inline qlonglong getSave() const { return _save; }
    inline qlonglong getCount() const { return _count; }

    static StaticsAccept *fromJson(QVariant var);

private:
    qulonglong _staticsId;
    QDateTime _date;
    Company _usedIn;
    Company _issuedBy;
    UseRule *_useRule;
    qlonglong _save;
    qlonglong _count;
};

#endif

