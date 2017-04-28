#include "issuedeductionrule.h"

IssueDeductionRule::IssueDeductionRule(qulonglong ruleId, QString &ruleName, qlonglong startingAmount, QList<Company> &validCompanies, int expiredMonths, qlonglong startingDeduction, qlonglong stepAmount, qlonglong stepDeduction) :
    IssueRule(ruleId, ruleName, startingAmount, validCompanies, expiredMonths),
    _startingDeduction(startingDeduction),
    _stepAmount(stepAmount),
    _stepDeduction(stepDeduction)
{
}

