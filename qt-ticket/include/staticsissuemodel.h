#ifndef STATICS_ISSUE_MODEL_H
#define STATICS_ISSUE_MODEL_H

#include "staticsissue.h"
#include <QAbstractTableModel>
#include <QStringList>

class StaticsIssueModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit StaticsIssueModel(const QList<StaticsIssue *> &staticsIssues, QObject* parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    QString exportToFile();

private:
    const QList<StaticsIssue *>  &_staticsIssues;
    QStringList _header;
};

#endif

