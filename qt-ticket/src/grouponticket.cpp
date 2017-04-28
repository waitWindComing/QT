#include "grouponticket.h"

GrouponTicket::GrouponTicket(qulonglong ticketId, Company company, QDateTime &issuedWhen, QDateTime &expiredWhen, qlonglong cost, qlonglong deduction, qlonglong leftDeduction) :
    Ticket(ticketId, company, issuedWhen, expiredWhen),
    _cost(cost),
    _deduction(deduction),
    _leftDeduction(leftDeduction),
    _leftDeductionAfterUse(leftDeduction)
{
}

GrouponTicket::~GrouponTicket()
{
}

