#include "issueruletablemodel.h"

IssueRuleTableModel::IssueRuleTableModel(const QList<IssueRule *> &issueRules, QObject* parent)
    :_issueRules(issueRules),
     QAbstractTableModel(parent)
{
    _header << trUtf8("类型") << trUtf8("名称") << trUtf8("消费额度") << trUtf8("优惠额度") << trUtf8("有效期") << trUtf8("选择");
}

int IssueRuleTableModel::rowCount(const QModelIndex& parent) const 
{
    return _issueRules.size();
}

int IssueRuleTableModel::columnCount(const QModelIndex& parent) const
{ 
    return 6;
}

QVariant IssueRuleTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section < 6 && role == Qt::DisplayRole) {
        return _header[section];
    } else {
        return QVariant();
    }
}

QVariant IssueRuleTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return QVariant();

    if (index.row() >= _issueRules.size()) return QVariant();

    IssueRule *issueRule = _issueRules.at(index.row());
    if (role == Qt::DisplayRole) {
        if (index.column() == 0) {
            if (IssueRule::Discount == issueRule->getType()) {
                return trUtf8("折扣券");
            } else {
                return trUtf8("代金券");
            }
        } else if (index.column() == 1) {
            return issueRule->getRuleName();
        } else if (index.column() == 2) {
            return QString("%1").arg(issueRule->getStartingAmount() / 100.0, 0, 'f', 2);
        } else if (index.column() == 3) {
            QString ruleinfo;
            if (IssueRule::Discount == issueRule->getType()) {
                ruleinfo = QString("%1%2%3:%4%\n%5").arg(trUtf8("满")).arg(issueRule->getStartingAmount() / 100.0, 0, 'f', 2).arg(trUtf8("折扣率")).arg(issueRule->getDiscount()).arg(issueRule->getOnce() ? trUtf8("一次性") : trUtf8("反复使用"));
            } else {
                ruleinfo = QString("%1%2%3:%4\n%5%6%7%8").arg(trUtf8("满")).arg(issueRule->getStartingAmount() / 100.0, 0, 'f', 2).arg(trUtf8("抵扣")).arg(issueRule->getStartingDeduction() / 100.0, 0, 'f', 2).arg(trUtf8("之后每")).arg(issueRule->getStepAmount() / 100.0, 0, 'f', 2).arg(trUtf8("返")).arg(issueRule->getStepDeduction() / 100.0, 0, 'f', 2);
            }
            return ruleinfo;
        } else if (index.column() == 4) {
            return issueRule->getExpiredMonths();
        } else {        // radiobox
            return QVariant();
        }
    }

    if (role == Qt::CheckStateRole) {
        return issueRule->getCheckState();
    }

    return QVariant();
}

Qt::ItemFlags IssueRuleTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    if (index.column() == 5) {
        return Qt::ItemIsEnabled | Qt::ItemIsEditable;
    } else {
        return Qt::ItemIsEnabled;
    }
}

bool IssueRuleTableModel::setData ( const QModelIndex & index, const QVariant & value, int role )
{
    Qt::CheckState state = static_cast<Qt::CheckState>(value.toInt());
    IssueRule *issueRule = _issueRules.at(index.row());
    issueRule->setCheckState(state);
    emit dataChanged(createIndex(index.row(), 0), createIndex(index.row(), 5));
    return true;
}

void IssueRuleTableModel::clearChecked()
{
    for (QList<IssueRule *>::const_iterator i = _issueRules.begin(); i != _issueRules.end(); ++i) {
        (*i)->setCheckState(Qt::Unchecked);
    }
}

