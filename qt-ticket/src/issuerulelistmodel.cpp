#include "issuerulelistmodel.h"

IssueRuleListModel::IssueRuleListModel(QList<IssueRule *> &rules, QObject* parent)
    :_rules(rules),
     QAbstractListModel(parent)
{
}

int IssueRuleListModel::rowCount(const QModelIndex& parent) const 
{
    return _rules.size();
}

QVariant IssueRuleListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return QVariant();

    if (index.row() >= _rules.size()) return QVariant();

    if (role == Qt::DisplayRole)
        return _rules.at(index.row())->getRuleName();

    return QVariant();
}

void IssueRuleListModel::itemAppended()
{
    beginInsertRows(QModelIndex(), _rules.size(), _rules.size());
    endInsertRows();
}

void IssueRuleListModel::refresh()
{
    emit layoutChanged();
}

