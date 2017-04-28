#ifndef USE_DISCOUNT_RULE_H
#define USE_DISCOUNT_RULE_H

#include "userule.h"

class UseDiscountRule : public UseRule {
public:
    UseDiscountRule(qulonglong ruleId, QString &ruleName, int leastConsume, int maxDeduction, QList<Company> &issuedByCompanies, int discountType, int discount, double convertRate);

    inline RuleType getType() const { return Discount; }
    inline int getDiscountType() const { return _discountType; }
    inline int getDiscount() const { return _discount; }
    inline double getConvertRate() const { return _convertRate; }
    inline int getRate() const { return 0; }

private:
    int _discountType;
    int _discount;
    double _convertRate;
};

#endif

