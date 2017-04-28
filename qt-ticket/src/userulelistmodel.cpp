#include "userulelistmodel.h"

UseRuleListModel::UseRuleListModel(QList<UseRule *> &rules, QObject* parent)
    :_rules(rules),
     QAbstractListModel(parent)
{
}

int UseRuleListModel::rowCount(const QModelIndex& parent) const 
{
    return _rules.size();
}

QVariant UseRuleListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return QVariant();

    if (index.row() >= _rules.size()) return QVariant();

    if (role == Qt::DisplayRole)
        return _rules.at(index.row())->getRuleName();

    return QVariant();
}

void UseRuleListModel::itemAppended()
{
    beginInsertRows(QModelIndex(), _rules.size(), _rules.size());
    endInsertRows();
}

void UseRuleListModel::refresh()
{
    emit layoutChanged();
}
