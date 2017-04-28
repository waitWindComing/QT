#include "issuerule.h"
#include "issuedeductionrule.h"
#include "issuediscountrule.h"
#include <QVariant>
#include <qDebug>

IssueRule::IssueRule(qulonglong ruleId, QString &ruleName, qlonglong startingAmount, QList<Company> &validCompanies, int expiredMonths)
    :_ruleId(ruleId),
     _ruleName(ruleName),
     _startingAmount(startingAmount),
     _validCompanies(validCompanies),
     _expiredMonths(expiredMonths),
     _state(Qt::Unchecked)
{
}

IssueRule::~IssueRule()
{
}

IssueRule *IssueRule::fromJson(QVariant variant)
{
    QVariantMap var = variant.toMap();
    qulonglong ruleId = var["id"].toULongLong();
    QString ruleName = var["name"].toString();
    qlonglong startingAmount = var["startingAmount"].toLongLong();
    bool isEnabled = var["isEnabled"].toBool();
    if (!isEnabled) {
        return NULL;
    }
    int expiredMonths = var["expiredMonths"].toInt();
    QVariantList companies = var["validCompanies"].toList();
    QList<Company> validCompanies;
    for (QVariantList::iterator tt = companies.begin(); tt != companies.end(); ++tt) {
        validCompanies.append(Company::fromJson(*tt));
    }

    if (var.contains("startingDeduction")) {
        qlonglong startingDeduction = var["startingDeduction"].toLongLong();
        qlonglong stepAmount = var["stepAmount"].toLongLong();
        qlonglong stepDeduction = var["stepDeduction"].toLongLong();
        return new IssueDeductionRule(ruleId, ruleName, startingAmount, validCompanies, expiredMonths, startingDeduction, stepAmount, stepDeduction);
    } else {
        int discount = var["discount"].toInt();
        bool once = var["once"].toBool();
        return new IssueDiscountRule(ruleId, ruleName, startingAmount, validCompanies, expiredMonths, discount, once);
    }
}

