#include "useruletablemodel.h"
#include <qDebug>

UseRuleTableModel::UseRuleTableModel(const QList<UseRule *> &useRules, QObject* parent)
    :_useRules(useRules),
     QAbstractTableModel(parent)
{
    _header << trUtf8("名称") << trUtf8("最低消费") << trUtf8("最大抵扣") << trUtf8("类型") << trUtf8("信息") << trUtf8("选择");
}

int UseRuleTableModel::rowCount(const QModelIndex& parent) const 
{
    return _useRules.size();
}

int UseRuleTableModel::columnCount(const QModelIndex& parent) const
{ 
    return 6;
}

QVariant UseRuleTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section < 6 && role == Qt::DisplayRole) {
        return _header[section];
    } else {
        return QVariant();
    }
}

QVariant UseRuleTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return QVariant();

    if (index.row() >= _useRules.size()) return QVariant();

    UseRule *useRule = _useRules.at(index.row());
    if (role == Qt::DisplayRole) {
        if (index.column() == 0) {
            return useRule->getRuleName();
        } else if (index.column() == 1) {
            QString rst;
            return rst.setNum(useRule->getLeastConsume() / 100.0, 'f', 2);
        } else if (index.column() == 2) {
            QString rst;
            return rst.setNum(useRule->getMaxDeduction() / 100.0, 'f', 2);
        } else if (index.column() == 3) {
            if (UseRule::Discount == useRule->getType()) {
                return trUtf8("折扣券");
            } else {
                return trUtf8("代金券");
            }
        } else if (index.column() == 4) {
            QString ruleinfo;
            if (UseRule::Discount == useRule->getType()) {
                if (useRule->getDiscountType() == 0) {
                    ruleinfo = QString("%1:%2").arg(trUtf8("折扣转换系数")).arg(useRule->getConvertRate());
                } else {
                    ruleinfo = QString("%1:%2%").arg(trUtf8("新折扣率")).arg(useRule->getDiscount());
                }
            } else {
                ruleinfo = QString("%1:%2%").arg(trUtf8("抵扣率")).arg(useRule->getRate());
            }
            return ruleinfo;
        } else {        // radiobox
            return QVariant();
        }
    }

    if (role == Qt::CheckStateRole) {
        return useRule->getCheckState();
    }

    return QVariant();
}

Qt::ItemFlags UseRuleTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    if (index.column() == 5) {
        return Qt::ItemIsEnabled | Qt::ItemIsEditable;
    } else {
        return Qt::ItemIsEnabled;
    }
}

bool UseRuleTableModel::setData ( const QModelIndex & index, const QVariant & value, int role )
{
    Qt::CheckState state = static_cast<Qt::CheckState>(value.toInt());
    UseRule *useRule = _useRules.at(index.row());
    useRule->setCheckState(state);
    emit dataChanged(createIndex(index.row(), 0), createIndex(index.row(), 6));
    return true;
}

void UseRuleTableModel::clearChecked()
{
    for (QList<UseRule *>::const_iterator i = _useRules.begin(); i != _useRules.end(); ++i) {
        (*i)->setCheckState(Qt::Unchecked);
    }
}

void UseRuleTableModel::itemAppended()
{
    beginInsertRows(QModelIndex(), _useRules.size(), _useRules.size());
    endInsertRows();
}

void UseRuleTableModel::refresh()
{
    emit layoutChanged();
}
