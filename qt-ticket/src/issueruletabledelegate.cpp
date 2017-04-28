#include "issueruletabledelegate.h"
#include "issueruletablemodel.h"
#include <QPainter>
#include <QApplication>
#include <qDebug>

IssueRuleTableDelegate::IssueRuleTableDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void IssueRuleTableDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    painter->save();
    Qt::CheckState state = static_cast<Qt::CheckState>(index.model()->data(index, Qt::CheckStateRole).toInt());
    if (state == Qt::Checked) {
        painter->setPen(QColor(0, 0, 255));
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
    } else if (index.column() == 4) {
        int expiredMonths = qvariant_cast<int>(index.data());
        QString expiredInfo;
        switch (expiredMonths) {
        case 1:
            expiredInfo = trUtf8("一个月");
            break;
        case 2:
            expiredInfo = trUtf8("两个月");
            break;
        case 3:
            expiredInfo = trUtf8("三个月");
            break;
        case 6:
            expiredInfo = trUtf8("半年");
            break;
        case 12:
            expiredInfo = trUtf8("一年");
            break;
        case 24:
            expiredInfo = trUtf8("两年");
            break;
        case 36:
            expiredInfo = trUtf8("三年");
            break;
        default:
            expiredInfo = trUtf8("永远有效");
            break;
        }
        painter->drawText(option.rect, Qt::AlignCenter, expiredInfo);
    } else {
        painter->drawText(option.rect, Qt::AlignCenter, qvariant_cast<QString>(index.data()));
    }
    painter->restore();
}

bool IssueRuleTableDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::MouseButtonRelease && index.column() == 5) {
        Qt::CheckState state = static_cast<Qt::CheckState>(index.model()->data(index, Qt::CheckStateRole).toInt());
        if (Qt::Unchecked == state) {
            ((IssueRuleTableModel *)model)->clearChecked();
            model->setData(index, Qt::Checked, Qt::EditRole);
            emit ruleChanged();
        }
    }
    return true;
}

