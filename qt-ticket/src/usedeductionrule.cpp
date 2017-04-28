#include "usedeductionrule.h"

UseDeductionRule::UseDeductionRule(qulonglong ruleId, QString &ruleName, int leastConsume, int maxDeduction, QList<Company> &issuedByCompanies, int rate) :
    UseRule(ruleId, ruleName, leastConsume, maxDeduction, issuedByCompanies),
    _rate(rate)
{
}

