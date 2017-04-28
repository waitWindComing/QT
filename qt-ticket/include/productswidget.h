#ifndef PRODUCTS_WIDGET_H
#define PRODUCTS_WIDGET_H
#include "productwidget.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QList>

class ProductsPage;
class ProductsWidget : public QWidget
{
    Q_OBJECT

public:
    ProductsWidget(QList<CompanyProduct> &companyProducts, QMap<CompanyProduct, int> &selectedProducts, QWidget *parent = 0);
    QMap<CompanyProduct, int> getSelectedCompanyProducts();
    virtual QSize sizeHint() const;

private slots:
    void nextPage();
    void prevPage();

private:
    QMap<CompanyProduct, int> _selectedProducts;
    int _currentPage;
    int _totalPages;

    QVBoxLayout *_layout;
    QLabel *_pageLabel;
    QPushButton *_nextPageButton;
    QPushButton *_prevPageButton;
    ProductsPage *_productsPage;
};

#endif
