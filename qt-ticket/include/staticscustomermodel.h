#ifndef STATICS_CUSTOMER_MODEL_H
#define STATICS_CUSTOMER_MODEL_H

#include "staticscustomer.h"
#include <QAbstractTableModel>
#include <QStringList>

class StaticsCustomerModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit StaticsCustomerModel(const QList<StaticsCustomer *> &staticsCustomers, QObject* parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    QString exportToFile();

private:
    const QList<StaticsCustomer *>  &_staticsCustomers;
    QStringList _header;
};

#endif

