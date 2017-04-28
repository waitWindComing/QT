#include "staticsacceptmodel.h"
#include "userule.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>

StaticsAcceptModel::StaticsAcceptModel(const QList<StaticsAccept *> &staticsAccepts, StaticsType type, QObject* parent)
    :_staticsAccepts(staticsAccepts),
     _type(type),
     QAbstractTableModel(parent)
{
    if (ByAccept == type) {    // 本商户收券，包括合作商户发行的券
        _header << trUtf8("发行商户") << trUtf8("日期") << trUtf8("使用规则") << trUtf8("优惠券数量") << trUtf8("优惠额度");
    } else {  // 本商户发行的券，包括在合作商户的使用情况
        _header << trUtf8("接收商户") << trUtf8("日期") << trUtf8("使用规则") << trUtf8("优惠券数量") << trUtf8("优惠额度");
    }
}

int StaticsAcceptModel::rowCount(const QModelIndex& parent) const 
{
    return _staticsAccepts.size();
}

int StaticsAcceptModel::columnCount(const QModelIndex& parent) const
{ 
    return 5;
}

QVariant StaticsAcceptModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section < 5 && role == Qt::DisplayRole) {
        return _header[section];
    } else {
        return QVariant();
    }
}

QVariant StaticsAcceptModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return QVariant();

    if (index.row() >= _staticsAccepts.size()) return QVariant();

    StaticsAccept *staticsAccept = _staticsAccepts.at(index.row());
    if (role == Qt::DisplayRole) {
        if (index.column() == 0) {
            if (ByAccept == _type) {
                return QString("%1").arg(staticsAccept->getIssuedBy().getName());
            } else {
                return QString("%1").arg(staticsAccept->getUsedIn().getName());
            }
        } else if (index.column() == 1) {
            QDateTime d = staticsAccept->getDate();
            return d.toString("yyyy-M-d");
        } else if (index.column() == 2) {
            UseRule *useRule = staticsAccept->getUseRule();
            if (useRule) {
                return QString("%1").arg(useRule->getRuleName());
            } else {
                return QVariant();
            }
        } else if (index.column() == 3) {
            return QString("%1").arg(staticsAccept->getCount());
        } else if (index.column() == 4) {
            return QString("%1").arg(staticsAccept->getSave() / 100.0, 0, 'f', 2);
        } else {
            return QVariant();
        }
    }

    return QVariant();
}

QString StaticsAcceptModel::exportToFile()
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

    for (int row = 0; row < _staticsAccepts.size(); ++row) {
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

