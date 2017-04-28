#include "staticscustomermodel.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>

StaticsCustomerModel::StaticsCustomerModel(const QList<StaticsCustomer *> &staticsCustomers, QObject* parent)
    :_staticsCustomers(staticsCustomers),
     QAbstractTableModel(parent)
{
    _header << trUtf8("日期") << trUtf8("新用户数") << trUtf8("应收额度") << trUtf8("实收额度") << trUtf8("优惠额度");
}

int StaticsCustomerModel::rowCount(const QModelIndex& parent) const 
{
    return _staticsCustomers.size();
}

int StaticsCustomerModel::columnCount(const QModelIndex& parent) const
{ 
    return 5;
}

QVariant StaticsCustomerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section < 5 && role == Qt::DisplayRole) {
        return _header[section];
    } else {
        return QVariant();
    }
}

QVariant StaticsCustomerModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return QVariant();

    if (index.row() >= _staticsCustomers.size()) return QVariant();

    StaticsCustomer *staticsCustomer = _staticsCustomers.at(index.row());
    if (role == Qt::DisplayRole) {
        if (index.column() == 0) {
            QDateTime d = staticsCustomer->getDate();
            return d.toString("yyyy-M-d");
        } else if (index.column() == 1) {
            return QString("%1").arg(staticsCustomer->getRegs());
        } else if (index.column() == 2) {
            return QString("%1").arg(staticsCustomer->getAmount() / 100.0, 0, 'f', 2);
        } else if (index.column() == 3) {
            return QString("%1").arg(staticsCustomer->getNetPaid() / 100.0, 0, 'f', 2);
        } else if (index.column() == 4) {
            return QString("%1").arg((staticsCustomer->getAmount() - staticsCustomer->getNetPaid())/ 100.0, 0, 'f', 2);
        } else {
            return QVariant();
        }
    }

    return QVariant();
}

QString StaticsCustomerModel::exportToFile()
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

    for (int row = 0; row < _staticsCustomers.size(); ++row) {
        for (int column = 0; column < 5; ++column) {
            QModelIndex index = createIndex(row, column);
            QVariant d = data(index, Qt::DisplayRole);
            out << d.toString() << ",";
        }
        out << endl;
    }
    file.close();
    return outputFileName;
}

