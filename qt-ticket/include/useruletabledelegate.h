#ifndef USE_RULE_TABLE_DELEGATE_H
#define USE_RULE_TABLE_DELEGATE_H

#include <QStyledItemDelegate>

class UseRuleTableDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    UseRuleTableDelegate(QObject *parent = 0);
    void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

signals:
    void ruleChanged();
};

#endif

