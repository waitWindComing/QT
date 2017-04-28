#include "staticstransfer.h"
#include <QVariant>
#include <qDebug>

StaticsTransfer::StaticsTransfer(qulonglong staticsId, QDateTime date, qlonglong regs, qlonglong count)
    :_staticsId(staticsId),
     _date(date),
     _regs(regs),
     _count(count)
{
}

StaticsTransfer::~StaticsTransfer()
{
}

StaticsTransfer *StaticsTransfer::fromJson(QVariant var)
{
    qulonglong staticsId = var.toMap()["id"].toULongLong();
    QDateTime date = QDateTime::fromMSecsSinceEpoch(var.toMap()["date"].toULongLong());
    qlonglong regs = var.toMap()["regs"].toLongLong();
    qlonglong count = var.toMap()["count"].toLongLong();

    return new StaticsTransfer(staticsId, date, regs, count);
}

