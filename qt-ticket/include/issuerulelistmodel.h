#ifndef ISSUE_RULE_LIST_MODEL_H
#define ISSUE_RULE_LIST_MODEL_H

#include <QAbstractListModel>
#include "issuerule.h"

class IssueRuleListModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit IssueRuleListModel(QList<IssueRule *> &rules, QObject* parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    void itemAppended();
    void refresh();

private:
    QList<IssueRule *>  &_rules;
};

#endif

