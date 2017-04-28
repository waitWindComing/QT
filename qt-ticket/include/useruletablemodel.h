#ifndef USE_RULE_TABLE_MODEL_H
#define USE_RULE_TABLE_MODEL_H

#include <QAbstractTableModel>
#include "userule.h"

class UseRuleTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit UseRuleTableModel(const QList<UseRule *> &useRules, QObject* parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    void clearChecked();
    void itemAppended();
    void refresh();
    inline const QList<UseRule *> &getUseRules() const { return _useRules; }

private:
    const QList<UseRule *>  &_useRules;
    QStringList _header;
};

#endif


