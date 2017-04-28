#ifndef TICKET_H
#define TICKET_H
#include "company.h"
#include <QDateTime>
#include <QMetaType>

class Ticket {
public:
    enum TicketType { Discount, Deduction, Groupon };

    Ticket(qulonglong ticketId, Company company, QDateTime &issuedWhen, QDateTime &expiredWhen);
    virtual ~Ticket();

    inline qulonglong getTicketId() const { return _ticketId; }
    inline Company getCompany() const { return _company; }
    inline QDateTime getIssuedWhen() const { return _issuedWhen; }
    inline QDateTime getExpiredWhen() const { return _expiredWhen; }
    inline Qt::CheckState getCheckState() const { return _state; }
    inline void setCheckState(Qt::CheckState state) { _state = state; }
    inline bool getValidAfterUse() const { return _validAfterUse; }
    inline void setValidAfterUse(bool validAfterUse) { _validAfterUse = validAfterUse; }
    inline qlonglong getSaveAfterUse() const { return _save; }
    inline void setSaveAfterUse(int save) { _save = save; }

    virtual TicketType getType() const = 0;
    virtual qlonglong getCost() const = 0;
    virtual qlonglong getDeduction() const = 0;
    virtual qlonglong getLeftDeduction() const = 0;
    virtual qlonglong getLeftDeductionAfterUse() const = 0;
    virtual void setLeftDeductionAfterUse(qlonglong) = 0;
    virtual int getDiscount() const = 0;
    virtual bool getOnce() const = 0;

    static Ticket *fromJson(QVariant var);

private:
    qulonglong _ticketId;
    Company _company;
    QDateTime _issuedWhen;
    QDateTime _expiredWhen;
    Qt::CheckState _state;
    bool _validAfterUse;
    qlonglong _save;
};

Q_DECLARE_METATYPE(Ticket *)

#endif

