#include "booking.h"
#include <QVariant>

Booking::Booking(qulonglong bookingId, CompanyProduct &companyProduct, int quantity, QDateTime &bookingWhen)
    :_bookingId(bookingId),
     _companyProduct(companyProduct),
     _quantity(quantity),
     _bookingWhen(bookingWhen)
{
}

Booking Booking::fromJson(QVariant var)
{
    qulonglong bookingId = var.toMap()["id"].toULongLong();
    CompanyProduct companyProduct = CompanyProduct::fromJson(var.toMap()["companyProduct"]);
    int quantity = var.toMap()["quantity"].toInt();
    QDateTime bookingWhen = QDateTime::fromMSecsSinceEpoch(var.toMap()["bookingWhen"].toULongLong());
    return Booking(bookingId, companyProduct, quantity, bookingWhen);
}

