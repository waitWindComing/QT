#include "productswidget.h"
#include "productspage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <qDebug>

ProductsWidget::ProductsWidget(QList<CompanyProduct> &companyProducts, QMap<CompanyProduct, int> &selectedProducts, QWidget *parent) :
    _currentPage(0),
    QWidget(parent)
{
    QListIterator<CompanyProduct> it(companyProducts);
    while (it.hasNext()) {
        _selectedProducts.insert(it.next(), 0);
    }

    QMapIterator<CompanyProduct, int> i(selectedProducts);
    while (i.hasNext()) {
        i.next();
        _selectedProducts.insert(i.key(), i.value());
    }

    if (companyProducts.size() == 0) {
        QLabel *empty = new QLabel(trUtf8("没有可选择大产品！"), this);
        empty->setGeometry(0, 0, 200, 80);
    } else {
        _totalPages = (companyProducts.size() - 1) / (ProductsPage::COLUMN * ProductsPage::ROW) + 1;

        _layout = new QVBoxLayout;
        if (_totalPages > 1) {
            QHBoxLayout *ht = new QHBoxLayout;
            _nextPageButton = new QPushButton(trUtf8("下一页"), this);
            _prevPageButton = new QPushButton(trUtf8("上一页"), this);
            _pageLabel = new QLabel(QString("%1/%2").arg(_currentPage + 1).arg(_totalPages));
            ht->addWidget(_prevPageButton);
            ht->addWidget(_pageLabel);
            ht->addWidget(_nextPageButton);
            _layout->addLayout(ht);
            connect(_nextPageButton, SIGNAL(clicked()), this, SLOT(nextPage()));
            connect(_prevPageButton, SIGNAL(clicked()), this, SLOT(prevPage()));
        }
        _productsPage = new ProductsPage(_selectedProducts, _currentPage, this);
        _layout->addWidget(_productsPage);
        setLayout(_layout);
    }
}

QSize ProductsWidget::sizeHint() const
{
    if (_selectedProducts.size() == 0) {
        return QSize(500, 300);
    } else {
        return _productsPage->sizeHint() + QSize(0, 60);
    }
}

void ProductsWidget::nextPage()
{
    if (_currentPage == _totalPages - 1) return;
    _layout->removeWidget(_productsPage);
    delete _productsPage;
    ++_currentPage;
    _productsPage = new ProductsPage(_selectedProducts, _currentPage, this);
    _layout->insertWidget(0, _productsPage);
    _layout->update();
}

void ProductsWidget::prevPage()
{
    if (_currentPage == 0) return;
    _layout->removeWidget(_productsPage);
    delete _productsPage;
    --_currentPage;
    _productsPage = new ProductsPage(_selectedProducts, _currentPage, this);
    _layout->insertWidget(0, _productsPage);
    _layout->update();
}

QMap<CompanyProduct, int> ProductsWidget::getSelectedCompanyProducts()
{
    QMap<CompanyProduct, int> rst;
    QMapIterator<CompanyProduct, int> i(_selectedProducts);
    while (i.hasNext()) {
        i.next();
        if (i.value() != 0) {
            rst.insert(i.key(), i.value());
        }
    }
    return rst;
}

