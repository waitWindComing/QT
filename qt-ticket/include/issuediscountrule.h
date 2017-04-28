#ifndef ISSUE_DISCOUNT_RULE_H
#define ISSUE_DISCOUNT_RULE_H

#include "issuerule.h"

class IssueDiscountRule : public IssueRule {
public:
    IssueDiscountRule(qulonglong ruleId, QString &ruleName, int amount, QList<Company> &validCompanies, int expiredMonths, int discount, bool once);

    inline RuleType getType() const { return Discount; }
    inline int getDiscount() const { return _discount; }
    inline bool getOnce() const { return _once; }
    inline qlonglong getStartingDeduction() const { return 0; }
    inline qlonglong getStepAmount() const { return 0; }
    inline qlonglong getStepDeduction() const { return 0; }

private:
    int _discount;
    bool _once;
};

#endif

