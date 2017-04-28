#ifndef PRODUCT_WIDGET_H
#define PRODUCT_WIDGET_H
#include "companyproduct.h"
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class ProductWidget : public QWidget
{
    Q_OBJECT

public:
    ProductWidget(CompanyProduct &companyProduct, int count, QWidget *parent = 0);

signals:
    void countChanged(CompanyProduct&, int);

private slots:
    void decrease();
    void increase();
    void loadLogoFinished();

private:
    CompanyProduct _companyProduct;
    int _count;

    void loadLogo();

    QLabel *_nameLabel;
    QLabel *_logoLabel;
    QLabel *_priceLabel;
    QLabel *_quantityLabel;
    QPushButton *_increaseButton;
    QPushButton *_decreaseButton;

    QNetworkAccessManager _qnam;
    QNetworkReply *_reply;
};

#endif
