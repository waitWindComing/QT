#include "productselectiondlg.h"
#include "productswidget.h"
#include "booking.h"
#include "utils.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QProgressBar>
#include <QDialogButtonBox>
#include <QTimer>
#include <QJson/Serializer>
#include <QJson/Parser>
#include <qDebug>

ProductSelectionDlg::ProductSelectionDlg(const qulonglong &managerId, const qulonglong &companyId, const QString &token, QMap<CompanyProduct, int> &selectedCompanyProducts, QString &customer, QWidget *parent)
    : _managerId(managerId),
    _companyId(companyId),
    _token(token),
    _selectedCompanyProducts(selectedCompanyProducts),
    _customer(customer),
    QDialog(parent)
{
    setWindowTitle(trUtf8("选择商品"));
    _layout = new QVBoxLayout;

    _waitingBar = new QProgressBar(this);
    _waitingBar->setFixedSize(QSize(240, 30));
    _waitingBar->setMinimum(0);
    _waitingBar->setMaximum(0);
    _layout->addWidget(_waitingBar);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal, this);
    QPushButton *okButton = new QPushButton(trUtf8("确认"), this);
    buttonBox->addButton(okButton, QDialogButtonBox::AcceptRole);
    QPushButton *cancleButton = new QPushButton(trUtf8("取消"), this);
    buttonBox->addButton(cancleButton, QDialogButtonBox::RejectRole);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    _layout->addWidget(buttonBox);
    setLayout(_layout);

    QNetworkRequest request(QUrl((Utils::getHost().append("/companies/%1/companyProducts")).arg(_companyId)));
    _companyProductReply = _qnam.get(request);
    connect(_companyProductReply, SIGNAL(finished()), this, SLOT(handleGetCompanyProductsFinished()));
    _isCompanyProductFinished = false;

    if (_selectedCompanyProducts.size() == 0 && _customer.length() == 11) {
        QNetworkRequest bookingRequest(QUrl((Utils::getHost().append("/managers/%1/bookings?customerName=%2&timestamp=%3")).arg(_managerId).arg(_customer).arg(QDateTime::currentMSecsSinceEpoch())));
        bookingRequest.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
        _bookingReply = _qnam.get(bookingRequest);
        connect(_bookingReply, SIGNAL(finished()), this, SLOT(handleGetBookingsFinished()));
        _isBookingFinished = false;
    } else {
        _isBookingFinished = true;
    }
}

QMap<CompanyProduct, int> ProductSelectionDlg::getSelectedCompanyProducts()
{
    return _productsWidget->getSelectedCompanyProducts();
}

void ProductSelectionDlg::handleGetCompanyProductsFinished()
{
    qDebug() << "handleGetCompanyProductsFinished()";
    _isCompanyProductFinished = true;
    if (_companyProductReply != NULL) {
        _companyProductReply->close();
        _companyProductReply->deleteLater();
        if (_companyProductReply->error() == QNetworkReply::NoError) {
            QByteArray json = _companyProductReply->readAll();
            QJson::Parser parser;
            bool ok;
            QVariantList products = parser.parse (json, &ok).toList();
            if (!ok) {
                qDebug() << "some error happend on server";
                return;
            }
            for (QVariantList::iterator i = products.begin(); i != products.end(); ++i) {
                _companyProducts.append(CompanyProduct::fromJson(*i));
            }
        }
    }
    if (_isCompanyProductFinished && _isBookingFinished) {
        createProductsWidget();
    }
}

void ProductSelectionDlg::handleGetBookingsFinished()
{
    qDebug() << "handleGetBookingsFinished()";
    _isBookingFinished = true;
    if (_bookingReply != NULL) {
        _bookingReply->close();
        _bookingReply->deleteLater();
        if (_bookingReply->error() == QNetworkReply::NoError) {
            QByteArray json = _bookingReply->readAll();
            QJson::Parser parser;
            bool ok;
            QVariantList bookings = parser.parse (json, &ok).toList();
            if (!ok) {
                qDebug() << "some error happend on server";
                return;
            }
            for (QVariantList::iterator i = bookings.begin(); i != bookings.end(); ++i) {
                Booking booking = Booking::fromJson(*i);
                _selectedCompanyProducts.insert(booking.getCompanyProduct(), booking.getQuantity());
            }
        }
    }
    if (_isCompanyProductFinished && _isBookingFinished) {
        createProductsWidget();
    }
}

void ProductSelectionDlg::createProductsWidget()
{
    _layout->removeWidget(_waitingBar);
    delete _waitingBar;
    _productsWidget = new ProductsWidget(_companyProducts, _selectedCompanyProducts, this);
    _layout->insertWidget(0, _productsWidget);
    _layout->update();
    QTimer::singleShot(0, this, SLOT(resizeForNew()));
}

void ProductSelectionDlg::resizeForNew()
{
    QSize size = _layout->sizeHint();
    resize(size);
    QRect scr = QApplication::desktop()->screenGeometry();
    move(scr.center() - rect().center());
}
