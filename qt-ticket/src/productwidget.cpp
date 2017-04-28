#include "productwidget.h"
#include "utils.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

ProductWidget::ProductWidget(CompanyProduct &companyProduct, int count, QWidget *parent) :
    _companyProduct(companyProduct),
    _count(count),
    QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;

    _nameLabel = new QLabel(this);
    _nameLabel->setText(_companyProduct.getProduct().getName());
    _nameLabel->setGeometry(0, 0, 220, 30);
    layout->addWidget(_nameLabel);

    QHBoxLayout *h1 = new QHBoxLayout;
    _logoLabel = new QLabel(this);
    _logoLabel->setPixmap(QPixmap(":/ticket/100x100.gif"));
    _logoLabel->setGeometry(0, 0, 100, 100);
    h1->addWidget(_logoLabel);

    QVBoxLayout *v = new QVBoxLayout;
    _priceLabel = new QLabel(QString("%1").arg(_companyProduct.getCurrentPrice().getPrice() / 100.0, 0, 'f', 2), this);
    v->addWidget(_priceLabel);

    QHBoxLayout *h2 = new QHBoxLayout;
    _decreaseButton = new QPushButton(trUtf8("<"), this);
    h2->addWidget(_decreaseButton);

    _quantityLabel = new QLabel(QString("%1").arg(_count), this);
    h2->addWidget(_quantityLabel);
    _increaseButton = new QPushButton(trUtf8(">"), this);
    h2->addWidget(_increaseButton);

    v->addLayout(h2);

    h1->addLayout(v);
    layout->addLayout(h1);
    setLayout(layout);

    connect(_decreaseButton, SIGNAL(clicked()), this, SLOT(decrease()));
    connect(_increaseButton, SIGNAL(clicked()), this, SLOT(increase()));

    loadLogo();
}

void ProductWidget::loadLogo()
{
    QNetworkRequest request(QUrl((Utils::getHost().append("/products/%1/logo")).arg(_companyProduct.getProduct().getProductId())));
    _reply = _qnam.get(request);
    connect(_reply, SIGNAL(finished()), this, SLOT(loadLogoFinished()));
}

void ProductWidget::loadLogoFinished()
{
    if (_reply != NULL) {
        _reply->close();
        _reply->deleteLater();
        if (_reply->error() == QNetworkReply::NoError) {
            QByteArray imgData = _reply->readAll();
            QPixmap pixmap;
            pixmap.loadFromData(imgData);
            _logoLabel->setPixmap(pixmap);
        }
    }
}

void ProductWidget::decrease()
{
    if (_count > 0) {
        --_count;
        _quantityLabel->setText(QString("%1").arg(_count));
        emit countChanged(_companyProduct, _count);
    }
}

void ProductWidget::increase()
{
    ++_count;
    _quantityLabel->setText(QString("%1").arg(_count));
    emit countChanged(_companyProduct, _count);
}

