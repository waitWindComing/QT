#include "useticketdelegate.h"
#include <QPainter>
#include <QApplication>
#include <QMouseEvent>
#include <QDateTime>
#include <qDebug>

UseTicketDelegate::UseTicketDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void UseTicketDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    painter->save();
    Qt::CheckState state = static_cast<Qt::CheckState>(index.model()->data(index, Qt::CheckStateRole).toInt());
    if (state == Qt::Checked) {
        painter->setPen(QColor(0, 169, 162));
    }
    bool validAfterUse = static_cast<bool>(index.model()->data(index, Qt::UserRole).toBool());
    if (!validAfterUse) {
        painter->setPen(QColor(220, 178, 80));
    }
    if (index.column() == 6) {
        QStyleOptionButton check_box_style_option;
        check_box_style_option.state |= QStyle::State_Enabled;
        if (state == Qt::Checked) {
            check_box_style_option.state |= QStyle::State_On;
        } else {
            check_box_style_option.state |= QStyle::State_Off;
        }
        check_box_style_option.rect = option.rect.adjusted(15, 0, -15, 0);
        QApplication::style()->drawControl(QStyle::CE_CheckBox, &check_box_style_option, painter);
    } else if (index.column() == 5) {
        QDateTime dt = index.data().toDateTime();
        painter->drawText(option.rect, Qt::AlignCenter, dt.toString("yyyy-M-d\nh:m:s"));
    } else {
        painter->drawText(option.rect, Qt::AlignCenter, qvariant_cast<QString>(index.data()));
    }
    painter->restore();
}

bool UseTicketDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::MouseButtonRelease && index.column() == 6) {
        Qt::CheckState state = static_cast<Qt::CheckState>(index.model()->data(index, Qt::CheckStateRole).toInt());
        if (Qt::Checked == state) {
            model->setData(index, Qt::Unchecked, Qt::EditRole);
        } else {
            model->setData(index, Qt::Checked, Qt::EditRole);
        }
        emit editingInputted();
    }
    return true;
}

