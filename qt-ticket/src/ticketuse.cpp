#include "ticketuse.h"
#include "ticket.h"

TicketUse::TicketUse(qulonglong ticketUseId, Ticket *ticket, qlonglong benifit)
    :_ticketUseId(ticketUseId),
     _ticket(ticket),
     _benifit(benifit)
{
}

TicketUse::~TicketUse()
{
    delete _ticket;
}

TicketUse *TicketUse::fromJson(QVariant var)
{
    qulonglong ticketUseId = var.toMap()["id"].toULongLong();
    qlonglong benifit = var.toMap()["benifit"].toLongLong();
    Ticket *ticket = Ticket::fromJson(var.toMap()["ticket"]);
    return new TicketUse(ticketUseId, ticket, benifit);
}

