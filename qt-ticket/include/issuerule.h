#ifndef ISSUE_RULE_H
#define ISSUE_RULE_H
#include "company.h"
#include <QMetaType>

class IssueRule {
public:
    enum RuleType { Discount, Deduction };

    IssueRule(qulonglong ruleId, QString &ruleName, qlonglong amount, QList<Company> &validCompanies, int expiredMonths);
    virtual ~IssueRule();

    inline qulonglong getRuleId() const {return _ruleId; }
    inline QString getRuleName() const { return _ruleName; }
    inline qlonglong getStartingAmount() const { return _startingAmount; }
    inline QList<Company> getValidCompanies() const { return _validCompanies; }
    inline int getExpiredMonths() const { return _expiredMonths; }
    inline Qt::CheckState getCheckState() const { return _state; }
    inline void setCheckState(Qt::CheckState state) { _state = state; }

    virtual RuleType getType() const = 0;
    virtual qlonglong getStartingDeduction() const = 0;
    virtual qlonglong getStepAmount() const = 0;
    virtual qlonglong getStepDeduction() const = 0;
    virtual int getDiscount() const = 0;
    virtual bool getOnce() const = 0;

    static IssueRule *fromJson(QVariant var);

private:
    qulonglong     _ruleId;
    QString        _ruleName;
    qlonglong      _startingAmount;
    QList<Company> _validCompanies;
    int            _expiredMonths;
    Qt::CheckState _state;
};

Q_DECLARE_METATYPE(IssueRule *)

#endif

