#include "staticsissue.h"
#include "issuerule.h"
#include <QVariant>

StaticsIssue::StaticsIssue(qulonglong staticsId, QDateTime date, IssueRule *issueRule, qlonglong amount, qlonglong count)
    :_staticsId(staticsId),
     _date(date),
     _issueRule(issueRule),
     _amount(amount),
     _count(count)
{
}

StaticsIssue::~StaticsIssue()
{
    delete _issueRule;
}

StaticsIssue *StaticsIssue::fromJson(QVariant var)
{
    qulonglong staticsId = var.toMap()["id"].toULongLong();
    QDateTime date = QDateTime::fromMSecsSinceEpoch(var.toMap()["date"].toULongLong());
    IssueRule *issueRule = IssueRule::fromJson(var.toMap()["issueRule"]);
    qlonglong amount = var.toMap()["amount"].toLongLong();
    qlonglong count = var.toMap()["count"].toLongLong();

    return new StaticsIssue(staticsId, date, issueRule, amount, count);
}


