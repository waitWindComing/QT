#ifndef CONFIRM_CODE_WIDGET_H
#define CONFIRM_CODE_WIDGET_H
#include <QLineEdit>
#include <QPushButton>
#include <QProgressDialog>
#include <QNetworkReply>
#include <QNetworkAccessManager>

class ConfirmCodeWidget : public QWidget
{
    Q_OBJECT

public:
    ConfirmCodeWidget(const qulonglong &managerId, const QString &token, qulonglong customerId, QWidget *parent = 0);
    void focus();

signals:
    void skip();
    void pass();

private slots:
    void check();
    void handleGetConfirmCodeFinished();

private:
    const qulonglong &_managerId;
    const QString &_token;
    const qulonglong _customerId;
    QLineEdit *_codeEdit;
    QPushButton *_skipButton;
    QPushButton *_okButton;
    QProgressDialog *_waitingDialog;

    QNetworkAccessManager _qnam;
    QNetworkReply *_getConfirmCodeReply;
};


#endif

