#include "useruletabledelegate.h"
#include "useruletablemodel.h"
#include <QPainter>
#include <QApplication>
#include <qDebug>

UseRuleTableDelegate::UseRuleTableDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void UseRuleTableDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    painter->save();
    Qt::CheckState state = static_cast<Qt::CheckState>(index.model()->data(index, Qt::CheckStateRole).toInt());
    if (state == Qt::Checked) {
        painter->setPen(QColor(0, 169, 162));
    }
    if (index.column() == 5) {
        QStyleOptionButton check_box_style_option;
        check_box_style_option.state |= QStyle::State_Enabled;
        if (state == Qt::Checked) {
            check_box_style_option.state |= QStyle::State_On;
        } else {
            check_box_style_option.state |= QStyle::State_Off;
        }
        check_box_style_option.rect = option.rect.adjusted(15, 0, -15, 0);
        QApplication::style()->drawControl(QStyle::CE_RadioButton, &check_box_style_option, painter);
    } else {
        painter->drawText(option.rect, Qt::AlignCenter, qvariant_cast<QString>(index.data()));
    }
    painter->restore();
}

bool UseRuleTableDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::MouseButtonRelease && index.column() == 5) {
        Qt::CheckState state = static_cast<Qt::CheckState>(index.model()->data(index, Qt::CheckStateRole).toInt());
        if (Qt::Unchecked == state) {
            ((UseRuleTableModel *)model)->clearChecked();
            model->setData(index, Qt::Checked, Qt::EditRole);
            emit ruleChanged();
        }
    }
    return true;
}

