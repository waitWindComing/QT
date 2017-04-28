#include "consume.h"
#include "companycustomer.h"
#include "ticket.h"
#include "ticketuse.h"

Consume::Consume(qulonglong consumeId, qlonglong amount, qlonglong netPaid, qlonglong preBalance, qlonglong netDeduction, qlonglong postBalance, CompanyCustomer *companyCustomer, QDateTime &consumeWhen, QList<Ticket *> &tickets, QList<TicketUse *> &ticketUses)
    :_consumeId(consumeId),
     _amount(amount),
     _netPaid(netPaid),
     _preBalance(preBalance),
     _netDeduction(netDeduction),
     _postBalance(postBalance),
     _companyCustomer(companyCustomer),
     _consumeWhen(consumeWhen),
     _tickets(tickets),
     _ticketUses(ticketUses)
{
}

Consume::~Consume()
{
    delete _companyCustomer;
    for (QList<Ticket *>::iterator it = _tickets.begin(); it != _tickets.end(); ++it) {
        delete *it;
    }
    for (QList<TicketUse *>::iterator it = _ticketUses.begin(); it != _ticketUses.end(); ++it) {
        delete *it;
    }
}

Consume *Consume::fromJson(QVariant var)
{
    qulonglong consumeId = var.toMap()["id"].toULongLong();
    qlonglong amount = var.toMap()["amount"].toLongLong();
    qlonglong netPaid = var.toMap()["netPaid"].toLongLong();
    qlonglong preBalance = var.toMap()["preBalance"].toLongLong();
    qlonglong netDeduction = var.toMap()["netDeduction"].toLongLong();
    qlonglong postBalance = var.toMap()["postBalance"].toLongLong();
    CompanyCustomer *companyCustomer = CompanyCustomer::fromJson(var.toMap()["companyCustomer"]);
    QDateTime consumeWhen = QDateTime::fromMSecsSinceEpoch(var.toMap()["consumeWhen"].toULongLong());
    QList<Ticket *> tickets;
    QVariantList ticketsVar = var.toMap()["tickets"].toList();
    for (int i = 0; i < ticketsVar.size(); ++i) {
        Ticket *ticket = Ticket::fromJson(ticketsVar.at(i));
        tickets.append(ticket);
    }
    QList<TicketUse *> ticketUses;
    QVariantList ticketUsesVar = var.toMap()["uses"].toList();
    for (int i = 0; i < ticketUsesVar.size(); ++i) {
        TicketUse *ticketUse = TicketUse::fromJson(ticketUsesVar.at(i));
        ticketUses.append(ticketUse);
    }
    return new Consume(consumeId, amount, netPaid, preBalance, netDeduction, postBalance, companyCustomer, consumeWhen, tickets, ticketUses);
}

