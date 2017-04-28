#include "availableticketmodel.h"
#include <qDebug>

AvailableTicketModel::AvailableTicketModel(const QList<Ticket *> &tickets, QObject* parent)
    :_tickets(tickets),
     QAbstractTableModel(parent)
{
    _header << trUtf8("号码") << trUtf8("发行商户") << trUtf8("类型") << trUtf8("信息") << trUtf8("发行日期") << trUtf8("有效期") << trUtf8("选择");
}

int AvailableTicketModel::rowCount(const QModelIndex& parent) const 
{
    return _tickets.size();
}

int AvailableTicketModel::columnCount(const QModelIndex& parent) const
{ 
    return 7;
}

QVariant AvailableTicketModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section < 7 && role == Qt::DisplayRole) {
        return _header[section];
    } else {
        return QVariant();
    }
}

QVariant AvailableTicketModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return QVariant();

    if (index.row() >= _tickets.size()) return QVariant();

    Ticket *ticket = _tickets.at(index.row());
    if (role == Qt::DisplayRole) {
        if (index.column() == 0) {
            return ticket->getTicketId();
        } else if (index.column() == 1) {
            Company company = ticket->getCompany();
            return company.getName();
        } else if (index.column() == 2) {
            if (Ticket::Discount == ticket->getType()) {
                return trUtf8("折扣券");
            } else if(Ticket::Groupon == ticket->getType()) {
                return trUtf8("团购券");
            } else {
                return trUtf8("代金券");
            }
        } else if (index.column() == 3) {
            QString tickinfo;
            if (Ticket::Discount == ticket->getType()) {
                bool once = ticket->getOnce();
                if (once) {
                    tickinfo = trUtf8("折扣率:") + QString("%1\n%2").arg(ticket->getDiscount()).arg(trUtf8("一次性"));
                } else {
                    tickinfo = trUtf8("折扣率:") + QString("%1\n%2").arg(ticket->getDiscount()).arg(trUtf8("反复使用"));
                }
            } else if(Ticket::Groupon == ticket->getType()) {
                tickinfo = QString("%1:%2\n%3:%4").arg(trUtf8("团购金额")).arg(ticket->getCost() / 100.0, 0, 'f', 2).arg(trUtf8("面值")).arg(ticket->getLeftDeduction() / 100.0, 0, 'f', 2);
            } else {
                tickinfo = QString("%1:%2\n%3:%4").arg(trUtf8("抵扣金额")).arg(ticket->getDeduction() / 100.0, 0, 'f', 2).arg(trUtf8("剩余金额")).arg(ticket->getLeftDeduction() / 100.0, 0, 'f', 2);
            }
            return tickinfo;
        } else if (index.column() == 4) {
            return ticket->getIssuedWhen();
        } else if (index.column() == 5) {
            return ticket->getExpiredWhen();
        } else {        // checkbox
            return QVariant();
        }
    }

    if (role == Qt::CheckStateRole) {
        return ticket->getCheckState();
    }

    return QVariant();
}

Qt::ItemFlags AvailableTicketModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    if (index.column() == 6) {
        return Qt::ItemIsEnabled | Qt::ItemIsEditable;
    } else {
        return Qt::ItemIsEnabled;
    }
}

bool AvailableTicketModel::setData ( const QModelIndex & index, const QVariant & value, int role )
{
    Qt::CheckState state = static_cast<Qt::CheckState>(value.toInt());
    Ticket *ticket = _tickets.at(index.row());
    ticket->setCheckState(state);
    emit dataChanged(createIndex(index.row(), 0), createIndex(index.row(), 6));
    return true;
}

void AvailableTicketModel::itemAppended()
{
    beginInsertRows(QModelIndex(), _tickets.size(), _tickets.size());
    endInsertRows();
}

void AvailableTicketModel::refresh()
{
    emit layoutChanged();
}
