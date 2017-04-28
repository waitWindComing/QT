#ifndef _USE_TICKET_MODEL_H
#define _USE_TICKET_MODEL_H

#include <QAbstractTableModel>
#include "ticket.h"

class UseRule;
class UseTicketModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit UseTicketModel(const QList<Ticket *> &tickets, qlonglong due, QObject* parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    qlonglong calSaveWithUseRule(UseRule *useRule);
    inline const QList<Ticket *> &getTickets() const { return _tickets; }

private:
    qlonglong calSaveWithoutRule();
    qlonglong calSaveWithDiscountRule(UseRule *);
    qlonglong calSaveWithDeductionRule(UseRule *);
    void resetSaveAfterUse();

    UseRule *_useRule;
    const qlonglong _due;
    const QList<Ticket *>  &_tickets;
    QStringList _header;
};

#endif

