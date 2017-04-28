#include "usediscountrule.h"

UseDiscountRule::UseDiscountRule(qulonglong ruleId, QString &ruleName, int leastConsume, int maxDeduction, QList<Company> &issuedByCompanies, int discountType, int discount, double convertRate) :
    UseRule(ruleId, ruleName, leastConsume, maxDeduction, issuedByCompanies),
    _discountType(discountType),
    _discount(discount),
    _convertRate(convertRate)
{
}

