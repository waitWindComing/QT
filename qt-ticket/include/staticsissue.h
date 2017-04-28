#ifndef STATICS_ISSUE_H
#define STATICS_ISSUE_H
#include <QDateTime>

class IssueRule;
class StaticsIssue {
public:
    StaticsIssue(qulonglong staticsId, QDateTime date, IssueRule *issueRule, qlonglong amount, qlonglong count);
    ~StaticsIssue();

    inline qulonglong getStaticsId() const { return _staticsId; }
    inline QDateTime getDate() const { return _date; }
    inline IssueRule *getIssueRule() const { return _issueRule; }
    inline qlonglong getAmount() const { return _amount; }
    inline qlonglong getCount() const { return _count; }

    static StaticsIssue *fromJson(QVariant var);

private:
    qulonglong _staticsId;
    QDateTime _date;
    IssueRule *_issueRule;
    qlonglong _amount;
    qlonglong _count;
};

#endif

