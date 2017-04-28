#ifndef PRODUCT_SELECTION_DLG_H
#define PRODUCT_SELECTION_DLG_H

#include "companyproduct.h"

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class ProductsWidget;
class ProductSelectionDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ProductSelectionDlg(const qulonglong &managerId, const qulonglong &companyId, const QString &token, QMap<CompanyProduct, int> &selectedCompanyProducts, QString &customer, QWidget *parent = 0);

    QMap<CompanyProduct, int> getSelectedCompanyProducts();

private slots:
    void resizeForNew();
    void handleGetCompanyProductsFinished();
    void handleGetBookingsFinished();

private:
    void createProductsWidget();

    QList<CompanyProduct> _companyProducts;
    QMap<CompanyProduct, int> _selectedCompanyProducts;
    QString _customer;
    bool _isCompanyProductFinished;
    bool _isBookingFinished;

    QPushButton *_okButton;
    QVBoxLayout *_layout;
    QProgressBar *_waitingBar;
    ProductsWidget *_productsWidget;

    QNetworkAccessManager _qnam;
    QNetworkReply *_companyProductReply;
    QNetworkReply *_bookingReply;
    const qulonglong &_managerId;
    const qulonglong &_companyId;
    const QString &_token;
};

#endif
