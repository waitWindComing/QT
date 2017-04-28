#ifndef COMPANY_MODEL_H
#define COMPANY_MODEL_H

#include <QAbstractListModel>
#include "company.h"

class CompanyModel : public QAbstractListModel {
    Q_OBJECT

public:
    explicit CompanyModel(QList<Company> &companies, QObject* parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    void itemAppended();
    void refresh();

private:
    QList<Company>  &_companies;
};

#endif

