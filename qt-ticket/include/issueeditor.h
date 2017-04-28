#ifndef ISSUE_EDITOR_H
#define ISSUE_EDITOR_H

#include <QWidget>
#include <QTableView>

class IssueRule;
class IssueRuleTableModel;
class IssueRuleTableDelegate;
class IssueEditor : public QWidget
{
    Q_OBJECT

public:
    explicit IssueEditor(const qulonglong &managerId, const QString &managerName, const int &level, const QString &token, const qulonglong &companyId, const QString &companyName, const double &latitude, const double &longitude, const QList<IssueRule *> &issueRules, qlonglong netPay, QWidget *parent = 0);
    ~IssueEditor();
    IssueRule *getSelectedIssueRule();
    inline int getCandidateRulesCount() { return _issueRules.count(); }

private:
    static bool ruleLessThan(IssueRule *r1, IssueRule *r2);

    QTableView *_rulesView;
    IssueRuleTableModel *_model;
    IssueRuleTableDelegate *_ruleDelegate;

    const qulonglong &_managerId;
    const QString &_managerName;
    const int &_level;
    const QString &_token;
    const qulonglong &_companyId;
    const QString &_companyName;
    const double &_latitude;
    const double &_longitude;
    const qlonglong _netPay;
    QList<IssueRule *> _issueRules;
};


#endif

