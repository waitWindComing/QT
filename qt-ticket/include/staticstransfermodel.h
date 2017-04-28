#ifndef STATICS_TRANSFER_MODEL_H
#define STATICS_TRANSFER_MODEL_H

#include "staticstransfer.h"
#include <QAbstractTableModel>
#include <QStringList>

class StaticsTransferModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit StaticsTransferModel(const QList<StaticsTransfer *> &staticsTransfers, QObject* parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    QString exportToFile();

private:
    const QList<StaticsTransfer *>  &_staticsTransfers;
    QStringList _header;
};

#endif

