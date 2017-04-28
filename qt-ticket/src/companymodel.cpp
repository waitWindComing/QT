#include "companymodel.h"

CompanyModel::CompanyModel(QList<Company> &companies, QObject* parent)
    :_companies(companies),
     QAbstractListModel(parent)
{
}

int CompanyModel::rowCount(const QModelIndex& parent) const 
{
    return _companies.size();
}

QVariant CompanyModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return QVariant();

    if (index.row() >= _companies.size()) return QVariant();

    if (role == Qt::DisplayRole)
        return _companies.at(index.row()).getName();

    return QVariant();
}

void CompanyModel::itemAppended()
{
    beginInsertRows(QModelIndex(), _companies.size(), _companies.size());
    endInsertRows();
}

void CompanyModel::refresh()
{
    emit layoutChanged();
}
