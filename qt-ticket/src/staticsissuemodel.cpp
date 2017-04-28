#include "staticsissuemodel.h"
#include "issuerule.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>

StaticsIssueModel::StaticsIssueModel(const QList<StaticsIssue *> &staticsIssues, QObject* parent)
    :_staticsIssues(staticsIssues),
     QAbstractTableModel(parent)
{
    _header << trUtf8("日期") << trUtf8("发行规则") << trUtf8("发行数量") << trUtf8("发行额度");
}

int StaticsIssueModel::rowCount(const QModelIndex& parent) const 
{
    return _staticsIssues.size();
}

int StaticsIssueModel::columnCount(const QModelIndex& parent) const
{ 
    return 4;
}

QVariant StaticsIssueModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section < 4 && role == Qt::DisplayRole) {
        return _header[section];
    } else {
        return QVariant();
    }
}

QVariant StaticsIssueModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return QVariant();

    if (index.row() >= _staticsIssues.size()) return QVariant();

    StaticsIssue *staticsIssue = _staticsIssues.at(index.row());
    if (role == Qt::DisplayRole) {
        IssueRule *issueRule = staticsIssue->getIssueRule();
        if (index.column() == 0) {
            QDateTime d = staticsIssue->getDate();
            return d.toString("yyyy-M-d");
        } else if (index.column() == 1) {
            return QString("%1").arg(issueRule->getRuleName());
        } else if (index.column() == 2) {
            return QString("%1").arg(staticsIssue->getCount());
        } else if (index.column() == 3) {
            if (issueRule->getType() == IssueRule::Deduction) {
                return QString("%1").arg(staticsIssue->getAmount() / 100.0, 0, 'f', 2);
            } else {
                return QVariant();
            }
        } else {
            return QVariant();
        }
    }

    return QVariant();
}

QString StaticsIssueModel::exportToFile()
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

    for (int row = 0; row < _staticsIssues.size(); ++row) {
        for (int column = 0; column < 4; ++column) {
            QModelIndex index = createIndex(row, column);
            QVariant d = data(index, Qt::DisplayRole);
            out << d.toString() << ",";
        }
        out << endl;
    }
    file.close();
    return outputFileName;
}

