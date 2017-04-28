#include "discountticket.h"

DiscountTicket::DiscountTicket(qulonglong ticketId, Company company, QDateTime &issuedWhen, QDateTime &expiredWhen, int discount, bool once) :
    Ticket(ticketId, company, issuedWhen, expiredWhen),
    _discount(discount),
    _once(once)
{
}

DiscountTicket::~DiscountTicket()
{
}

