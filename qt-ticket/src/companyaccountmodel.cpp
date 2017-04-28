#include "companyaccountmodel.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>

CompanyAccountModel::CompanyAccountModel(const QList<CompanyAccount *> &companyAccounts, QObject* parent)
    :_companyAccounts(companyAccounts),
     QAbstractTableModel(parent)
{
    _header << trUtf8("日期") << trUtf8("操作员ID") << trUtf8("收券ID") << trUtf8("用户") << trUtf8("存入") << trUtf8("支出") << trUtf8("余额");
}

int CompanyAccountModel::rowCount(const QModelIndex& parent) const 
{
    return _companyAccounts.size();
}

int CompanyAccountModel::columnCount(const QModelIndex& parent) const
{ 
    return 7;
}

QVariant CompanyAccountModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section < 7 && role == Qt::DisplayRole) {
        return _header[section];
    } else {
        return QVariant();
    }
}

QVariant CompanyAccountModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return QVariant();

    if (index.row() >= _companyAccounts.size()) return QVariant();

    CompanyAccount *companyAccount = _companyAccounts.at(index.row());
    if (role == Qt::DisplayRole) {
        if (index.column() == 0) {
            QDateTime t = companyAccount->getTime();
            return t.toString("yyyy-M-d h:m");
        } else if (index.column() == 1) {
            if (companyAccount->getManagerId() != 0L) {
                return QString("%1").arg(companyAccount->getManagerId());
            } else if (companyAccount->getTicketId() == 0L) {
                return trUtf8("昊通信盟");
            } else {
                return QVariant();
            }
        } else if (index.column() == 2) {
            if (companyAccount->getTicketId() != 0L) {
                return QString("%1").arg(companyAccount->getTicketId());
            } else {
                return QVariant();
            }
        } else if (index.column() == 3) {
            if (companyAccount->getTicketId() != 0L) {
                return companyAccount->getCustomer();
            } else {
                return QVariant();
            }
        } else if (index.column() == 4) {
            if (companyAccount->getAmount() > 0) {
                return QString("%1").arg(companyAccount->getAmount() / 100.0, 0, 'f', 2);
            } else {
                return QVariant();
            }
        } else if (index.column() == 5) {
            if (companyAccount->getAmount() < 0) {
                return QString("%1").arg(companyAccount->getAmount() / 100.0, 0, 'f', 2);
            } else {
                return QVariant();
            }
        } else if (index.column() == 6) {
            return QString("%1").arg(companyAccount->getPostBalance() / 100.0, 0, 'f', 2);
        } else {
            return QVariant();
        }
    }

    return QVariant();
}

QString CompanyAccountModel::exportToFile()
{
    QString outputFileName = QDir::homePath() + "/" + QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz") + ".csv";
    QFile file(outputFileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return outputFileName;

    QTextStream out(&file);
    out.setCodec("gb18030");
    QStringList::const_iterator it;
    for (it = _header.constBegin(); it != _header.constEnd(); ++it) {
        out << *it << ",";
    }
    out << endl;

    for (int row = 0; row < _companyAccounts.size(); ++row) {
        for (int column = 0; column < 7; ++column) {
            QModelIndex index = createIndex(row, column);
            QVariant d = data(index, Qt::DisplayRole);
            out << d.toString() << ",";
        }
        out << endl;
    }
    file.close();
    return outputFileName;
}


