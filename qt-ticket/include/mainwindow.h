#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class MainWindow;
}

class PayWidget;
class PrepayWidget;
class QueryWidget;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(qulonglong managerId, QString &managerName, int level, QString &token, qulonglong companyId, QString &companyName, double latitude, double longitude, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_payButton_clicked();
    void on_prepayButton_clicked();
    void on_queryButton_clicked();

    void handleHttpLogoutFinished();
    void handleHttpBalanceFinished();

private:
    void requestBalance();
    void removeWidgets();

    Ui::MainWindow *ui;
    QPushButton *_payButton;
    QPushButton *_prepayButton;
    QPushButton *_queryButton;
    PayWidget *_payWidget;
    PrepayWidget *_prepayWidget;
    QueryWidget *_queryWidget;

    qulonglong _managerId;
    QString _managerName;
    int _level;
    QString _token;
    qulonglong _companyId;
    QString _companyName;
    double _latitude;
    double _longitude;

    QNetworkAccessManager _qnam;
    QNetworkReply *_logoutReply;
    QNetworkReply *_balanceReply;

    static const QString activeStyle;
    static const QString inactiveStyle;
};

#endif // MAINWINDOW_H
