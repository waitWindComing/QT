#ifndef _TICKET_USE_H
#define _TICKET_USE_H
#include <QWidget>
#include <QVariant>

class Ticket;
class TicketUse {
public:
    TicketUse(qulonglong ticketUseId, Ticket *ticket, qlonglong benifit);
    ~TicketUse();

    inline qulonglong getTicketUseId() const { return _ticketUseId; }
    inline Ticket * getTicket() const { return _ticket; }
    inline qlonglong getBenifit() const { return _benifit; }

    static TicketUse *fromJson(QVariant var);

private:
    qulonglong _ticketUseId;
    Ticket * _ticket;
    qlonglong _benifit;
};

#endif

