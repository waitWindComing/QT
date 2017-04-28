#ifndef USE_RULE_H
#define USE_RULE_H
#include "company.h"
#include <QMetaType>

class UseRule {
public:
    enum RuleType { Discount, Deduction };

    UseRule(qulonglong ruleId, QString &ruleName, int leastConsume, int maxDeduction, QList<Company> &issuedByCompanies);
    virtual ~UseRule();

    inline qulonglong getRuleId() const {return _ruleId; }
    inline QString getRuleName() const { return _ruleName; }
    inline qlonglong getLeastConsume() const { return _leastConsume; }
    inline qlonglong getMaxDeduction() const { return _maxDeduction; }
    inline QList<Company> getIssuedByCompanies() const { return _issuedByCompanies; }
    inline Qt::CheckState getCheckState() const { return _state; }
    inline void setCheckState(Qt::CheckState state) { _state = state; }

    virtual RuleType getType() const = 0;
    virtual int getDiscountType() const = 0;
    virtual int getDiscount() const = 0;
    virtual double getConvertRate() const = 0;
    virtual int getRate() const = 0;

    static UseRule *fromJson(QVariant var);

private:
    qulonglong     _ruleId;
    QString        _ruleName;
    qlonglong      _leastConsume;
    qlonglong      _maxDeduction;
    QList<Company> _issuedByCompanies;
    Qt::CheckState _state;
};

Q_DECLARE_METATYPE(UseRule *)

#endif

