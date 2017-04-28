#ifndef GROUPON_TICKET_H
#define GROUPON_TICKET_H
#include "ticket.h"

class GrouponTicket : public Ticket {
public:
    GrouponTicket(qulonglong ticketId, Company company, QDateTime &issuedWhen, QDateTime &expiredWhen, qlonglong cost, qlonglong deduction, qlonglong leftDeduction);
    ~GrouponTicket();

    inline TicketType getType() const { return Groupon; }
    inline qlonglong getCost() const { return _cost; }
    inline qlonglong getDeduction() const { return _deduction; }
    inline qlonglong getLeftDeduction() const { return _leftDeduction; }
    inline qlonglong getLeftDeductionAfterUse() const { return _leftDeductionAfterUse; }
    inline void setLeftDeductionAfterUse(qlonglong leftDeductionAfterUse) { _leftDeductionAfterUse = leftDeductionAfterUse; }
    inline int getDiscount() const { return 0; }
    inline bool getOnce() const { return false; }

private:
    qlonglong _cost;
    qlonglong _deduction;
    qlonglong _leftDeduction;
    qlonglong _leftDeductionAfterUse;
};

#endif

