#ifndef STATICS_ACCEPT_MODEL_H
#define STATICS_ACCEPT_MODEL_H

#include "staticsaccept.h"
#include <QAbstractTableModel>
#include <QStringList>

class StaticsAcceptModel : public QAbstractTableModel {
    Q_OBJECT

public:
    enum StaticsType { ByAccept, ByIssue };

    explicit StaticsAcceptModel(const QList<StaticsAccept *> &staticsAccepts, StaticsType type, QObject* parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    QString exportToFile();

private:
    const QList<StaticsAccept *>  &_staticsAccepts;
    QStringList _header;
    StaticsType _type;
};

#endif

