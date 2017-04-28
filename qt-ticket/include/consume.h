#ifndef CONSUME_H
#define CONSUME_H
#include <QWidget>
#include <QVariant>
#include <QDateTime>

class CompanyCustomer;
class Ticket;
class TicketUse;
class Consume {
public:
    Consume(qulonglong consumeId, qlonglong amount, qlonglong netPaid, qlonglong preBalance, qlonglong netDeduction, qlonglong postBalance, CompanyCustomer *companyCustomer, QDateTime &consumeWhen, QList<Ticket *> &tickets, QList<TicketUse *> &ticketUses);
    ~Consume();

    inline qulonglong getConsumeId() const { return _consumeId; }
    inline qlonglong getAmount() const { return _amount; }
    inline qlonglong getNetPaid() const { return _netPaid; }
    inline qlonglong getPreBalance() const { return _preBalance; }
    inline qlonglong getNetDeduction() const { return _netDeduction; }
    inline qlonglong getPostBalance() const { return _postBalance; }
    inline CompanyCustomer *getCompanyCustomer() const { return _companyCustomer; }
    inline QDateTime getConsumeWhen() const { return _consumeWhen; }
    inline QList<Ticket *> getTickets() const { return _tickets; }
    inline QList<TicketUse *> getTicketUses() const { return _ticketUses; }

    static Consume *fromJson(QVariant var);

private:
    qulonglong _consumeId;
    qlonglong _amount;
    qlonglong _netPaid;
    qlonglong _preBalance;
    qlonglong _netDeduction;
    qlonglong _postBalance;
    CompanyCustomer *_companyCustomer;
    QDateTime _consumeWhen;
    QList<Ticket *> _tickets;
    QList<TicketUse *> _ticketUses;
};

#endif

