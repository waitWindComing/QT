#include "ticketusemodel.h"
#include "ticketuse.h"
#include "ticket.h"

TicketUseModel::TicketUseModel(const QList<TicketUse *> &ticketUses, QObject* parent)
    :_ticketUses(ticketUses),
     QAbstractTableModel(parent)
{
    _header << trUtf8("号码") << trUtf8("类型") << trUtf8("商户") << trUtf8("信息") << trUtf8("节省");;
}

int TicketUseModel::rowCount(const QModelIndex& parent) const 
{
    return _ticketUses.size();
}

int TicketUseModel::columnCount(const QModelIndex& parent) const
{ 
    return 5;
}

QVariant TicketUseModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section < 5 && role == Qt::DisplayRole) {
        return _header[section];
    } else {
        return QVariant();
    }
}

QVariant TicketUseModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return QVariant();

    if (index.row() >= _ticketUses.size()) return QVariant();

    TicketUse *ticketUse = _ticketUses.at(index.row());
    if (role == Qt::DisplayRole) {
        if (index.column() == 0) {
            return ticketUse->getTicket()->getTicketId();
        } else if (index.column() == 1) {
            if (Ticket::Discount == ticketUse->getTicket()->getType()) {
                return trUtf8("折扣券");
            } else if (Ticket::Groupon == ticketUse->getTicket()->getType()) {
                return trUtf8("团购券");
            } else {
                return trUtf8("代金券");
            }
        } else if (index.column() == 2) {
            return ticketUse->getTicket()->getCompany().getName();
        } else if (index.column() == 3) {
            QString tickinfo;
            if (Ticket::Discount == ticketUse->getTicket()->getType()) {
                bool once = ticketUse->getTicket()->getOnce();
                if (once) {
                    tickinfo = trUtf8("折扣率:") + QString("%1\n%2").arg(ticketUse->getTicket()->getDiscount()).arg(trUtf8("一次性"));
                } else {
                    tickinfo = trUtf8("折扣率:") + QString("%1\n%2").arg(ticketUse->getTicket()->getDiscount()).arg(trUtf8("反复使用"));
                }
            } else if (Ticket::Groupon == ticketUse->getTicket()->getType()) {
                tickinfo = QString("%1:%2\n%3:%4").arg(trUtf8("团购金额")).arg(ticketUse->getTicket()->getCost() / 100.0, 0, 'f', 2).arg(trUtf8("面值")).arg(ticketUse->getTicket()->getLeftDeduction() / 100.0, 0, 'f', 2);
            } else {
                tickinfo = QString("%1:%2\n%3:%4").arg(trUtf8("抵扣金额")).arg(ticketUse->getTicket()->getDeduction() / 100.0, 0, 'f', 2).arg(trUtf8("剩余金额")).arg(ticketUse->getTicket()->getLeftDeduction() / 100.0, 0, 'f', 2);
            }
            return tickinfo;
        } else if (index.column() == 4) {
            return QString("%1").arg(ticketUse->getBenifit() / 100.0, 0, 'f', 2);
        } else {
            return QVariant();
        }
    }

    return QVariant();
}


