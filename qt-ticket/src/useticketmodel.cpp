#include "useticketmodel.h"
#include "userule.h"

UseTicketModel::UseTicketModel(const QList<Ticket *> &tickets, qlonglong due, QObject* parent)
    :_tickets(tickets),
     _due(due),
     QAbstractTableModel(parent)
{
    _header << trUtf8("号码") << trUtf8("类型") << trUtf8("原状态") << trUtf8("优惠额度") << trUtf8("新状态") << trUtf8("有效期") << trUtf8("选择");
}

int UseTicketModel::rowCount(const QModelIndex& parent) const 
{
    return _tickets.size();
}

int UseTicketModel::columnCount(const QModelIndex& parent) const
{ 
    return 7;
}

QVariant UseTicketModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section < 7 && role == Qt::DisplayRole) {
        return _header[section];
    } else {
        return QVariant();
    }
}

QVariant UseTicketModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return QVariant();

    if (index.row() >= _tickets.size()) return QVariant();

    Ticket *ticket = _tickets.at(index.row());
    if (role == Qt::DisplayRole) {
        if (index.column() == 0) {
            return ticket->getTicketId();
        } else if (index.column() == 1) {
            if (Ticket::Discount == ticket->getType()) {
                return trUtf8("折扣券");
            } else if (Ticket::Groupon == ticket->getType()) {
                return trUtf8("团购券");
            } else {
                return trUtf8("代金券");
            }
        } else if (index.column() == 2) {
            QString tickinfo;
            if (Ticket::Discount == ticket->getType()) {
                bool once = ticket->getOnce();
                if (once) {
                    tickinfo = trUtf8("折扣率:") + QString("%1\n%2").arg(ticket->getDiscount()).arg(trUtf8("一次性"));
                } else {
                    tickinfo = trUtf8("折扣率:") + QString("%1\n%2").arg(ticket->getDiscount()).arg(trUtf8("反复使用"));
                }
            } else if (Ticket::Groupon == ticket->getType()) {
                tickinfo = QString("%1:%2\n%3:%4").arg(trUtf8("团购金额")).arg(ticket->getCost() / 100.0, 0, 'f', 2).arg(trUtf8("面值")).arg(ticket->getLeftDeduction() / 100.0, 0, 'f', 2);
            } else {
                tickinfo = QString("%1:%2\n%3:%4").arg(trUtf8("抵扣金额")).arg(ticket->getDeduction() / 100.0, 0, 'f', 2).arg(trUtf8("剩余金额")).arg(ticket->getLeftDeduction() / 100.0, 0, 'f', 2);
            }
            return tickinfo;
        } else if (index.column() == 3) {
            QString save;
            return save.setNum(ticket->getSaveAfterUse() / 100.0, 'f', 2);
        } else if (index.column() == 4) {
            QString tickinfo;
            if (Ticket::Discount == ticket->getType()) {
                bool once = ticket->getOnce();
                if (once) {
                    tickinfo = trUtf8("折扣率:") + QString("%1\n%2").arg(ticket->getDiscount()).arg(trUtf8("一次性"));
                } else {
                    tickinfo = trUtf8("折扣率:") + QString("%1\n%2").arg(ticket->getDiscount()).arg(trUtf8("反复使用"));
                }
            } else {
                tickinfo = QString("%1:%2\n%3:%4").arg(trUtf8("抵扣金额")).arg(ticket->getDeduction() / 100.0, 0, 'f', 2).arg(trUtf8("剩余金额")).arg(ticket->getLeftDeductionAfterUse() / 100.0, 0, 'f', 2);
            }
            return tickinfo;
        } else if (index.column() == 5) {
            return ticket->getExpiredWhen();
        } else {        // checkbox
            return QVariant();
        }
    } else if (role == Qt::CheckStateRole) {
        return ticket->getCheckState();
    } else if (role == Qt::UserRole) {
        return ticket->getValidAfterUse();
    } else {
        return QVariant();
    }
}

Qt::ItemFlags UseTicketModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    if (index.column() == 6) {
        return Qt::ItemIsEnabled | Qt::ItemIsEditable;
    } else {
        return Qt::ItemIsEnabled;
    }
}

bool UseTicketModel::setData ( const QModelIndex & index, const QVariant & value, int role )
{
    Qt::CheckState state = static_cast<Qt::CheckState>(value.toInt());
    Ticket *ticket = _tickets.at(index.row());
    ticket->setCheckState(state);
    if (Ticket::Groupon == ticket->getType()){
        if (state == Qt::Checked) {
            ticket->setSaveAfterUse(ticket->getLeftDeduction() - ticket->getCost());
            ticket->setLeftDeductionAfterUse(0);
            ticket->setValidAfterUse(false);
        } else {
            ticket->setValidAfterUse(true);
            ticket->setSaveAfterUse(0);
            ticket->setLeftDeductionAfterUse(ticket->getLeftDeduction());
        }
    }
    emit dataChanged(createIndex(index.row(), 0), createIndex(index.row(), 6));
    return true;
}

qlonglong UseTicketModel::calSaveWithUseRule(UseRule *useRule)
{
    _useRule = useRule;
    resetSaveAfterUse();
    qlonglong save = 0;
    if (_useRule == NULL) {
        save = calSaveWithoutRule();
    } else {
        if (UseRule::Discount == useRule->getType()) {
            save = calSaveWithDiscountRule(useRule);
        } else {
            save = calSaveWithDeductionRule(useRule);
        }
    }
    emit layoutChanged();
    return save;
}

qlonglong UseTicketModel::calSaveWithoutRule()
{
    qlonglong save = 0;
    for (QList<Ticket *>::const_iterator i = _tickets.begin(); i != _tickets.end(); ++i) {
        if ((*i)->getCheckState() != Qt::Checked) continue;
        if ((*i)->getType() == Ticket::Discount) {
           int discount = (*i)->getDiscount();
           bool once = (*i)->getOnce();
           if (once) {
               (*i)->setValidAfterUse(false);
           }
           save = _due - _due * discount / 100;
           (*i)->setSaveAfterUse(save);
           break;
        } else {
            qlonglong leftDeduction = (*i)->getLeftDeduction();
            if (save + leftDeduction > _due) {
                (*i)->setLeftDeductionAfterUse(leftDeduction - (_due - save));
                (*i)->setSaveAfterUse(_due - save);
                save = _due;
                break;   // save all, no pay
            } else {
                save += leftDeduction;
                (*i)->setSaveAfterUse(leftDeduction);
                (*i)->setLeftDeductionAfterUse(0);
                (*i)->setValidAfterUse(false);
            }
        }
    }
    return save;
}

qlonglong UseTicketModel::calSaveWithDiscountRule(UseRule *useRule)
{
    qlonglong save = 0;
    qlonglong leastConsume = useRule->getLeastConsume();
    qlonglong maxDeduction = useRule->getMaxDeduction();
    int discountType = useRule->getDiscountType();
    int discount = useRule->getDiscount();
    double convertRate = useRule->getConvertRate();
    if (_due < leastConsume) {
        return 0;
    }
    for (QList<Ticket *>::const_iterator i = _tickets.begin(); i != _tickets.end(); ++i) {
        if ((*i)->getCheckState() != Qt::Checked) continue;
        if ((*i)->getType() == Ticket::Discount) {
           int ticketDiscount = (*i)->getDiscount();
           bool once = (*i)->getOnce();
           if (once) {
               (*i)->setValidAfterUse(false);
           }
           if (discountType == 0) {   // discount = ticketDiscount * convertRate
               discount = ticketDiscount * convertRate;
           }
           save = _due - _due * discount / 100;
           if (save > maxDeduction) {
               save = maxDeduction;
           }
           (*i)->setSaveAfterUse(save);
           break;
        } else {
            continue;
        }
    }
    return save;
}

qlonglong UseTicketModel::calSaveWithDeductionRule(UseRule *useRule)
{
    qlonglong save = 0;
    qlonglong leastConsume = useRule->getLeastConsume();
    qlonglong maxDeduction = useRule->getMaxDeduction();
    if (_due < leastConsume) {
        return 0;
    }
    for (QList<Ticket *>::const_iterator i = _tickets.begin(); i != _tickets.end(); ++i) {
        if ((*i)->getCheckState() != Qt::Checked) continue;
        if ((*i)->getType() == Ticket::Deduction) {
            qlonglong leftDeduction = (*i)->getLeftDeduction();
            qlonglong maxSave = qMin(_due, maxDeduction);
            if (save + leftDeduction > maxSave) {
                (*i)->setLeftDeductionAfterUse(leftDeduction - (maxSave - save));
                save = maxSave;
                (*i)->setSaveAfterUse(save);
                break;
            } else {
                save += leftDeduction;
                (*i)->setSaveAfterUse(leftDeduction);
                (*i)->setLeftDeductionAfterUse(0);
                (*i)->setValidAfterUse(false);
            }
        } else {
            continue;
        }
    }
    return save;
}

void UseTicketModel::resetSaveAfterUse()
{
    for (QList<Ticket *>::const_iterator i = _tickets.begin(); i != _tickets.end(); ++i) {
        (*i)->setValidAfterUse(true);
        (*i)->setSaveAfterUse(0);
        (*i)->setLeftDeductionAfterUse((*i)->getLeftDeduction());
    }
}

