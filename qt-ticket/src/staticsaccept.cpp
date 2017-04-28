#include "staticsaccept.h"
#include "userule.h"
#include <QVariant>
#include <qDebug>

StaticsAccept::StaticsAccept(qulonglong staticsId, QDateTime date, Company usedIn, Company issuedBy, UseRule *useRule, qlonglong save, qlonglong count)
    :_staticsId(staticsId),
     _date(date),
     _usedIn(usedIn),
     _issuedBy(issuedBy),
     _useRule(useRule),
     _save(save),
     _count(count)
{
}

StaticsAccept::~StaticsAccept()
{
    if (_useRule) delete _useRule;
}

StaticsAccept *StaticsAccept::fromJson(QVariant var)
{
    qulonglong staticsId = var.toMap()["id"].toULongLong();
    QDateTime date = QDateTime::fromMSecsSinceEpoch(var.toMap()["date"].toULongLong());
    QVariant useRuleVar = var.toMap()["useRule"];
    UseRule *useRule = NULL;
    if (!useRuleVar.isNull()) {
        useRule = UseRule::fromJson(var.toMap()["useRule"]);
    }
    Company usedIn = Company::fromJson(var.toMap()["usedIn"]);
    Company issuedBy = Company::fromJson(var.toMap()["issuedBy"]);
    qlonglong save = var.toMap()["save"].toLongLong();
    qlonglong count = var.toMap()["count"].toLongLong();

    return new StaticsAccept(staticsId, date, usedIn, issuedBy, useRule, save, count);
}


