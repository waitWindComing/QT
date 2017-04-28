#include "issuediscountrule.h"


IssueDiscountRule::IssueDiscountRule(qulonglong ruleId, QString &ruleName, int amount, QList<Company> &validCompanies, int expiredMonths, int discount, bool once) :
    IssueRule(ruleId, ruleName, amount, validCompanies, expiredMonths),
    _discount(discount),
    _once(once)
{
}

