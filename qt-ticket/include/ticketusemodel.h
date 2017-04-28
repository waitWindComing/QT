#ifndef _TICKET_USE_MODEL_H
#define _TICKET_USE_MODEL_H

#include <QAbstractTableModel>
#include "ticketuse.h"

class TicketUseModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit TicketUseModel(const QList<TicketUse *> &ticketUses, QObject* parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    const QList<TicketUse *>  _ticketUses;
    QStringList _header;
};

#endif

