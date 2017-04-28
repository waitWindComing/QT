#ifndef PRODUCT_H
#define PRODUCT_H
#include <QWidget>
#include <QVariant>

class Product {
public:
    Product(qulonglong productId, QString &name, QString &unit, QString &contentType);

    inline qulonglong getProductId() const { return _productId; }
    inline QString getName() const { return _name; }
    inline QString getUnit() const { return _unit; }
    inline QString getContentType() const { return _contentType; }

    static Product fromJson(QVariant var);

private:
    qulonglong _productId;
    QString _name;
    QString _unit;
    QString _contentType;
};

#endif

