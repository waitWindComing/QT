#include "ticket.h"
#include "deductionticket.h"
#include "discountticket.h"
#include "grouponticket.h"
#include <QVariant>
#include <qDebug>

Ticket::Ticket(qulonglong ticketId, Company company, QDateTime &issuedWhen, QDateTime &expiredWhen) :
    _ticketId(ticketId),
    _company(company),
    _issuedWhen(issuedWhen),
    _expiredWhen(expiredWhen),
    _state(Qt::Unchecked),
    _save(0),
    _validAfterUse(true)
{
}

Ticket::~Ticket()
{
}

Ticket *Ticket::fromJson(QVariant variant)
{
    QVariantMap var = variant.toMap();
    qulonglong ticketId = var["id"].toULongLong();
    QVariant cvar = var["company"].toMap();
    Company company = Company::fromJson(cvar);
    QDateTime issuedWhen = QDateTime::fromMSecsSinceEpoch(var["issuedWhen"].toULongLong());
    QDateTime expiredWhen = QDateTime::fromMSecsSinceEpoch(var["expiredWhen"].toULongLong());
    if (var.contains("cost")) {
        int cost = var["cost"].toInt();
        int deduction = var["deduction"].toInt();
        int leftDeduction = var["leftDeduction"].toInt();
        return new GrouponTicket(ticketId, company, issuedWhen, expiredWhen, cost, deduction, leftDeduction);
    } else if (var.contains("deduction")) {
        int deduction = var["deduction"].toInt();
        int leftDeduction = var["leftDeduction"].toInt();
        return new DeductionTicket(ticketId, company, issuedWhen, expiredWhen, deduction, leftDeduction);
    } else {
        int discount = var["discount"].toInt();
        bool once = var["once"].toBool();
        return new DiscountTicket(ticketId, company, issuedWhen, expiredWhen, discount, once);
    }
}

