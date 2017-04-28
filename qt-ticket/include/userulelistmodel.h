#ifndef USE_RULE_LIST_MODEL_H
#define USE_RULE_LIST_MODEL_H

#include <QAbstractListModel>
#include "userule.h"

class UseRuleListModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit UseRuleListModel(QList<UseRule *> &rules, QObject* parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    void itemAppended();
    void refresh();

private:
    QList<UseRule *>  &_rules;
};

#endif

