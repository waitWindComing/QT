#ifndef ONLINE_LOGIN_H
#define ONLINE_LOGIN_H

#include <QDialog>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QProgressDialog>
#include <QPaintEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class OnlineLogin : public QDialog
{
    Q_OBJECT

public:
    explicit OnlineLogin(QWidget *parent = 0);
    ~OnlineLogin();

public slots:
    void accept();

protected:
    void paintEvent(QPaintEvent * event);
    void showEvent(QShowEvent *event);

private slots:
    void getCompany();
    void getManager();
    void handleGetCompanyFinished();
    void handleGetManagerFinished();
    void handleLoginFinished();

private:
    QLineEdit *_companyIdEdit;
    QLineEdit *_managerIdEdit;
    QLineEdit *_passwordEdit;
    QProgressDialog *_waitingDialog;
    QHBoxLayout *_companyNameLayout;
    QHBoxLayout *_managerNameLayout;

    QNetworkAccessManager _qnam;
    QNetworkReply *_companyReply;
    QNetworkReply *_managerReply;
    QNetworkReply *_loginReply;
};

#endif // ONLINE_LOGIN_H

