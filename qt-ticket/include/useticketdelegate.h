#ifndef _USE_TICKET_DELEGATE_H
#define _USE_TICKET_DELEGATE_H

#include <QStyledItemDelegate>

class UseTicketDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    UseTicketDelegate(QObject *parent = 0);
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

signals:
    void editingInputted();
};

#endif

