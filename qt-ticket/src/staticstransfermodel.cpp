#include "staticstransfermodel.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>

StaticsTransferModel::StaticsTransferModel(const QList<StaticsTransfer *> &staticsTransfers, QObject* parent)
    :_staticsTransfers(staticsTransfers),
     QAbstractTableModel(parent)
{
    _header << trUtf8("日期") << trUtf8("新用户数（通过转移产生）") << trUtf8("转移次数");
}

int StaticsTransferModel::rowCount(const QModelIndex& parent) const 
{
    return _staticsTransfers.size();
}

int StaticsTransferModel::columnCount(const QModelIndex& parent) const
{ 
    return 3;
}

QVariant StaticsTransferModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section < 3 && role == Qt::DisplayRole) {
        return _header[section];
    } else {
        return QVariant();
    }
}

QVariant StaticsTransferModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return QVariant();

    if (index.row() >= _staticsTransfers.size()) return QVariant();

    StaticsTransfer *staticsTransfer = _staticsTransfers.at(index.row());
    if (role == Qt::DisplayRole) {
        if (index.column() == 0) {
            QDateTime d = staticsTransfer->getDate();
            return d.toString("yyyy-M-d");
        } else if (index.column() == 1) {
            return QString("%1").arg(staticsTransfer->getRegs());
        } else if (index.column() == 2) {
            return QString("%1").arg(staticsTransfer->getCount());
        } else {
            return QVariant();
        }
    }

    return QVariant();
}

QString StaticsTransferModel::exportToFile()
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

    for (int row = 0; row < _staticsTransfers.size(); ++row) {
        for (int column = 0; column < 3; ++column) {
            QModelIndex index = createIndex(row, column);
            QVariant d = data(index, Qt::DisplayRole);
            out << d.toString() << ",";
        }
        out << endl;
    }
    file.close();
    return outputFileName;
}


