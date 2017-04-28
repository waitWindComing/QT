#ifndef PRODUCTS_PAGE_H
#define PRODUCTS_PAGE_H
#include "companyproduct.h"
#include <QWidget>
#include <QGridLayout>
#include <QHash>

class ProductsPage : public QWidget
{
    Q_OBJECT

public:
    ProductsPage(QMap<CompanyProduct, int> &selectedProducts, int currentPage, QWidget *parent = 0);
    virtual QSize sizeHint() const;

    const static int COLUMN;
    const static int ROW;

private slots:
    void handleCountChanged(CompanyProduct&, int);

private:
    QGridLayout *_layout;

    QMap<CompanyProduct, int> &_selectedProducts;
    int _currentPage;
};

#endif
