#ifndef BOOKING_H
#define BOOKING_H
#include <QWidget>
#include <QVariant>
#include <QDateTime>

#include "companyproduct.h"

class Booking {
public:
    Booking(qulonglong bookingId, CompanyProduct &companyProduct, int quantity, QDateTime &bookingWhen);
    inline qulonglong getBookingId() const { return _bookingId; }
    inline CompanyProduct getCompanyProduct() const { return _companyProduct; }
    inline int getQuantity() const { return _quantity; }
    inline QDateTime getBookingWhen() const { return _bookingWhen; }

    static Booking fromJson(QVariant var);

private:
    qulonglong _bookingId;
    CompanyProduct _companyProduct;
    int _quantity;
    QDateTime _bookingWhen;
};

#endif

