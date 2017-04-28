#ifndef AVAILABLE_TICKET_DELEGATE_H
#define AVAILABLE_TICKET_DELEGATE_H

#include <QStyledItemDelegate>

class Ticket;
class UseRule;
class AvailableTicketDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    AvailableTicketDelegate(const qulonglong &companyId, const QList<UseRule *> &useRules, QObject *parent = 0);
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

signals:
    void editingInputted(bool);

private:
    bool ticketAcceptable(Ticket *current);

    const qulonglong &_companyId;
    const QList<UseRule *> &_useRules;
};

#endif

