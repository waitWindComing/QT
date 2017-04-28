#ifndef USE_DEDUCTION_RULE_H
#define USE_DEDUCTION_RULE_H

#include "userule.h"

class UseDeductionRule : public UseRule {
public:
    UseDeductionRule(qulonglong ruleId, QString &ruleName, int leastConsume, int maxDeduction, QList<Company> &issuedByCompanies, int rate);

    inline RuleType getType() const { return Deduction; }
    inline int getDiscountType() const { return 0; }
    inline double getConvertRate() const { return 1.0; }
    inline int getDiscount() const { return 0; }
    inline int getRate() const { return _rate; }

private:
    int _rate;
};

#endif

