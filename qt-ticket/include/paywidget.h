#ifndef PAY_WIDGET_H
#define PAY_WIDGET_H
#include <QWidget>
#include <QToolBar>
#include <QAction>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class UseRule;
class IssueRule;
class Customer;
class Consume;
class Ticket;
class TipsWidget;
class StepIndicator;
class DueEditor;
class ReduceEditor;
class NetEditor;
class IssueEditor;
class ConfirmCodeWidget;
class PayWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PayWidget(qulonglong &managerId, QString &managerName, int &level, QString &token, qulonglong &companyId, QString &companyName, double &latitude, double &longitude, QWidget *parent = 0);
    ~PayWidget();

private slots:
    void goStepOne();
    void goStepTwo();
    void goStepThree();
    void goStepFour();
    void requestCustomer();
    void postCustomer();
    void postConsume();
    void handleDueEditingFinshed();
    void handleDueEditingUnFinshed();
    void handleGetUseRulesFinished();
    void handleGetIssueRulesFinished();
    void handleGetCustomerFinished();
    void handleGetTicketsFinished();
    void handlePostCustomerFinished();
    void handlePostConsumeFinished();
    void postTicket();
    void handlePostTicketFinished();
    void cancel();
    void handlePutTicketFinished();
    void handleSummaryUpdated(QString &);
    void handleConfirmCodePass();
    void handleConfirmCodeSkip();

private:
    void requestTickets();
    void requestUseRules();
    void requestIssueRules();
    void createStepIndicatorWithStepTwo();
    void createStepIndicatorWithoutStepTwo();
    void stepFour();
    QList<Ticket *> getSelectedTickets();
    QList<UseRule *> getCandidateUseRules();
    void initActionsForFirstStep();
    void initActionsForLastStep();
    void putTickets();
    void putTicket(Ticket *ticket, UseRule *useRule);
    Customer *_customer;
    Consume *_consume;
    QList<Ticket *> _tickets;
    QList<UseRule *> _useRules;
    QList<IssueRule *> _issueRules;
    bool _skipStepTwo;
    qlonglong _netPaid;

    StepIndicator *_stepIndicator;
    TipsWidget *_tipsWidget;
    QToolBar   *_buttonBar;
    QToolBar   *_infoBar;
    QLabel     *_payInfo;    // payment info showed on toolbar
    QAction    *_nextAction;
    QAction    *_previousAction;
    QAction    *_issueAction;
    QAction    *_cancelAction;

    DueEditor *_dueEditor;
    ReduceEditor *_reduceEditor;
    NetEditor *_netEditor;
    IssueEditor *_issueEditor;
    ConfirmCodeWidget *_confirmCodeWidget;

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
    QNetworkReply *_ticketsReply;
    QNetworkReply *_useRulesReply;
    QNetworkReply *_issueRulesReply;
    QNetworkReply *_postCustomerReply;
    QNetworkReply *_postConsumeReply;
    QNetworkReply *_postTicketReply;
    QNetworkReply *_putTicketReply;
};

#endif

