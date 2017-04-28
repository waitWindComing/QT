#ifndef AVAILABLE_TICKET_MODEL_H
#define AVAILABLE_TICKET_MODEL_H

#include <QAbstractTableModel>
#include "ticket.h"

class AvailableTicketModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit AvailableTicketModel(const QList<Ticket *> &tickets, QObject* parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    void itemAppended();
    void refresh();
    inline const QList<Ticket *> &getTickets() const { return _tickets; }

private:
    const QList<Ticket *>  &_tickets;
    QStringList _header;
};

#endif


