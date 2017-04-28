#include "productspage.h"
#include "productwidget.h"
#include <QLayoutItem>
#include <qDebug>

const int ProductsPage::COLUMN = 4;
const int ProductsPage::ROW = 2;

ProductsPage::ProductsPage(QMap<CompanyProduct, int> &selectedProducts, int currentPage, QWidget *parent) :
    _selectedProducts(selectedProducts),
    _currentPage(currentPage),
    QWidget(parent)
{
    _layout = new QGridLayout();
    QMapIterator<CompanyProduct, int> i(selectedProducts);
    int index = 0;
    while (i.hasNext()) {
        i.next();
        if (index >= currentPage * COLUMN * ROW && index < (currentPage + 1) * COLUMN * ROW) {
            CompanyProduct companyProduct = i.key();
            int count = i.value();
            ProductWidget *productWidget = new ProductWidget(companyProduct, count, this);
            productWidget->setFixedSize(220, 130);
            connect(productWidget, SIGNAL(countChanged(CompanyProduct&, int)), this, SLOT(handleCountChanged(CompanyProduct&, int)));
            _layout->addWidget(productWidget, index / COLUMN, index - index / COLUMN * COLUMN);
        }
        ++index;
    }
    setLayout(_layout);
}

QSize ProductsPage::sizeHint() const
{
    QSize productWidgetSize(220, 130);
    int numOfProducts = _selectedProducts.size();
    int totalPage = (numOfProducts - 1) / (ProductsPage::COLUMN * ProductsPage::ROW) + 1;
    if (_currentPage == totalPage - 1) {      // last page
        int numOfCurrent = numOfProducts - _currentPage * COLUMN * ROW;
        int rows = (numOfCurrent - 1) / COLUMN + 1;
        return QSize(COLUMN * productWidgetSize.width() + (COLUMN - 1) * _layout->horizontalSpacing(), 
                     rows * productWidgetSize.height() + (ROW - 1) * _layout->verticalSpacing());
    } else {
        return QSize(COLUMN * productWidgetSize.width() + (COLUMN - 1) * _layout->horizontalSpacing(), 
                     ROW * productWidgetSize.height() + (ROW - 1) * _layout->verticalSpacing());
    }
}

void ProductsPage::handleCountChanged(CompanyProduct &companyProduct, int count)
{
    _selectedProducts.insert(companyProduct, count);
}

