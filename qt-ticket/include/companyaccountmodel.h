#ifndef COMPANY_ACCOUNT_MODEL_H
#define COMPANY_ACCOUNT_MODEL_H
#include "companyaccount.h"
#include <QAbstractTableModel>
#include <QStringList>

class CompanyAccountModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit CompanyAccountModel(const QList<CompanyAccount *> &companyAccounts, QObject* parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    QString exportToFile();

private:
    const QList<CompanyAccount *>  &_companyAccounts;
    QStringList _header;
};


#endif

