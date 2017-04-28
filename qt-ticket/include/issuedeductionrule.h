#ifndef ISSUE_DEDUCTION_RULE_H
#define ISSUE_DEDUCTION_RULE_H

#include "issuerule.h"

class IssueDeductionRule : public IssueRule {
public:
    IssueDeductionRule(qulonglong ruleId, QString &ruleName, qlonglong startingAmount, QList<Company> &validCompanies, int expiredMonths, qlonglong startingDeduction, qlonglong stepAmount, qlonglong stepDeduction);

    inline RuleType getType() const { return Deduction; }
    inline qlonglong getStartingDeduction() const { return _startingDeduction; }
    inline qlonglong getStepAmount() const { return _stepAmount; }
    inline qlonglong getStepDeduction() const { return _stepDeduction; }
    inline int getDiscount() const { return 0; }
    inline bool getOnce() const { return false; }

private:
    qlonglong _startingDeduction;
    qlonglong _stepAmount;
    qlonglong _stepDeduction;
};

#endif

