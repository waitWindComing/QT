#include "userule.h"
#include "usedeductionrule.h"
#include "usediscountrule.h"
#include <QVariant>
#include <qDebug>

UseRule::UseRule(qulonglong ruleId, QString &ruleName, int leastConsume, int maxDeduction, QList<Company> &issuedByCompanies)
    :_ruleId(ruleId),
     _ruleName(ruleName),
     _leastConsume(leastConsume),
     _maxDeduction(maxDeduction),
     _issuedByCompanies(issuedByCompanies),
     _state(Qt::Unchecked)
{
}

UseRule::~UseRule()
{
}

UseRule *UseRule::fromJson(QVariant variant)
{
    QVariantMap var = variant.toMap();
    qulonglong ruleId = var["id"].toULongLong();
    QString ruleName = var["name"].toString();
    qlonglong leastConsume = var["leastConsume"].toLongLong();
    qlonglong maxDeduction = var["maxDeduction"].toLongLong();
    bool isEnabled = var["isEnabled"].toBool();
    if (!isEnabled) {
        return NULL;
    }
    QVariantList companies = var["issuedByCompanies"].toList();
    QList<Company> issuedByCompanies;
    for (QVariantList::iterator tt = companies.begin(); tt != companies.end(); ++tt) {
        issuedByCompanies.append(Company::fromJson(*tt));
    }

    if (var.contains("rate")) {
        int rate = var["rate"].toInt();
        return new UseDeductionRule(ruleId, ruleName, leastConsume, maxDeduction, issuedByCompanies, rate);
    } else {
        int discountType = var["discountType"].toInt();
        int discount = var["discount"].toInt();
        double convertRate = var["convertRate"].toDouble();
        return new UseDiscountRule(ruleId, ruleName, leastConsume, maxDeduction, issuedByCompanies, discountType, discount, convertRate);
    }
}

