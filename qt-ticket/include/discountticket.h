#ifndef DISCOUNT_TICKET_H
#define DISCOUNT_TICKET_H
#include "ticket.h"

class DiscountTicket : public Ticket {
public:
    DiscountTicket(qulonglong ticketId, Company company, QDateTime &issuedWhen, QDateTime &expiredWhen, int discount, bool once);
    ~DiscountTicket();

    inline TicketType getType() const { return Discount; }
    inline qlonglong getCost() const { return 0; }
    inline qlonglong getDeduction() const { return 0; }
    inline qlonglong getLeftDeduction() const { return 0; }
    inline qlonglong getLeftDeductionAfterUse() const { return 0; }
    inline void setLeftDeductionAfterUse(qlonglong leftDeductionAfterUse) { }
    inline int getDiscount() const { return _discount; }
    inline bool getOnce() const { return _once; }

private:
    int _discount;
    bool _once;
};

#endif

