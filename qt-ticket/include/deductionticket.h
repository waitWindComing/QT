#ifndef DEDUCTION_TICKET_H
#define DEDUCTION_TICKET_H
#include "ticket.h"

class DeductionTicket : public Ticket {
public:
    DeductionTicket(qulonglong ticketId, Company company, QDateTime &issuedWhen, QDateTime &expiredWhen, qlonglong deduction, qlonglong leftDeduction);
    ~DeductionTicket();

    inline TicketType getType() const { return Deduction; }
    inline qlonglong getCost() const { return 0; }
    inline qlonglong getDeduction() const { return _deduction; }
    inline qlonglong getLeftDeduction() const { return _leftDeduction; }
    inline qlonglong getLeftDeductionAfterUse() const { return _leftDeductionAfterUse; }
    inline void setLeftDeductionAfterUse(qlonglong leftDeductionAfterUse) { _leftDeductionAfterUse = leftDeductionAfterUse; }
    inline int getDiscount() const { return 0; }
    inline bool getOnce() const { return false; }

private:
    qlonglong _deduction;
    qlonglong _leftDeduction;
    qlonglong _leftDeductionAfterUse;
};

#endif

