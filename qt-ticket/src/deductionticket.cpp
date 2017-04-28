#include "deductionticket.h"

DeductionTicket::DeductionTicket(qulonglong ticketId, Company company, QDateTime &issuedWhen, QDateTime &expiredWhen, qlonglong deduction, qlonglong leftDeduction) :
    Ticket(ticketId, company, issuedWhen, expiredWhen),
    _deduction(deduction),
    _leftDeduction(leftDeduction),
    _leftDeductionAfterUse(leftDeduction)
{
}

DeductionTicket::~DeductionTicket()
{
}
