#ifndef _TICKET_MODEL_H
#define _TICKET_MODEL_H

#include <QAbstractTableModel>
#include "ticket.h"

class TicketModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit TicketModel(const QList<Ticket *> &tickets, QObject* parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    const QList<Ticket *>  _tickets;
    QStringList _header;
};

#endif


