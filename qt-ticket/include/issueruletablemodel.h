#ifndef ISSUE_RULE_TABLE_MODEL_H
#define ISSUE_RULE_TABLE_MODEL_H

#include <QAbstractTableModel>
#include "issuerule.h"

class IssueRuleTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit IssueRuleTableModel(const QList<IssueRule *> &issueRules, QObject* parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    void clearChecked();

private:
    const QList<IssueRule *>  &_issueRules;
    QStringList _header;
};

#endif

