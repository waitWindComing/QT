#ifndef REPORT_WIDGET_H
#define REPORT_WIDGET_H
#include <QWidget>
#include <QDateEdit>
#include <QTableView>
#include <QLabel>
#include <QAction>
#include <QProgressDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class TipsWidget;
class StaticsCustomer;
class StaticsIssue;
class StaticsAccept;
class StaticsTransfer;
class CompanyAccount;
class StaticsCustomerModel;
class StaticsIssueModel;
class StaticsAcceptModel;
class StaticsTransferModel;
class CompanyAccountModel;
class ReportWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ReportWidget(qulonglong &managerId, QString &managerName, int &level, QString &token, qulonglong &companyId, QString &companyName, double &latitude, double &longitude, QWidget *parent = 0);
    ~ReportWidget();

private slots:
    void requestStaticsCustomer();
    void handleGetCustomersFinished();
    void requestStaticsIssue();
    void handleGetIssuesFinished();
    void requestStaticsAcceptByAccept();
    void handleGetAcceptsByAcceptFinished();
    void requestStaticsAcceptByIssue();
    void handleGetAcceptsByIssueFinished();
    void requestStaticsTransfer();
    void handleGetTransfersFinished();
    void requestCompanyAccount();
    void handleGetCompanyAccountsFinished();
    void handleGetBalanceFinished();
    void handleDateChanged(const QDate &);
    void exportToFile();
    void back();

private:
    void requestBalance();
    void showResult();

    QList<StaticsCustomer *> _staticsCustomers;
    StaticsCustomerModel *_customerModel;
    QList<StaticsIssue *> _staticsIssues;
    StaticsIssueModel *_issueModel;
    QList<StaticsAccept *> _staticsAccepts;
    StaticsAcceptModel *_acceptModel;
    StaticsAcceptModel *_useModel;
    QList<StaticsTransfer *> _staticsTransfers;
    StaticsTransferModel *_transferModel;
    QList<CompanyAccount *> _companyAccounts;
    CompanyAccountModel *_companyAccountModel;
    int _currentTag;

    TipsWidget *_tipsWidget;
    QLabel     *_balanceLabel;
    QAction    *_customerAction;
    QAction    *_issueAction;
    QAction    *_acceptAction;
    QAction    *_useAction;
    QAction    *_transferAction;
    QAction    *_accountAction;
    QAction    *_exportAction;
    QAction    *_backAction;

    QLabel     *_startLabel;
    QLabel     *_endLabel;
    QDateEdit  *_startDate;
    QDateEdit  *_endDate;
    QTableView *_reportView;
    QProgressDialog *_waitingDialog;

    const qulonglong &_managerId;
    const QString &_managerName;
    const int &_level;
    const QString &_token;
    const qulonglong &_companyId;
    const QString &_companyName;
    const double &_latitude;
    const double &_longitude;

    QNetworkAccessManager _qnam;
    QNetworkReply *_getCustomerReply;
    QNetworkReply *_getIssueReply;
    QNetworkReply *_getAcceptReply;
    QNetworkReply *_getUseReply;
    QNetworkReply *_getTransferReply;
    QNetworkReply *_getCompanyAccountReply;
    QNetworkReply *_getBalanceReply;
};

#endif

