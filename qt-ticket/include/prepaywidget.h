#ifndef PREPAY_WIDGET_H
#define PREPAY_WIDGET_H
#include <QWidget>
#include <QToolBar>
#include <QAction>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class CompanyCustomer;
class TipsWidget;
class StepIndicator;
class DueEditor;
class PreNetEditor;
class ConfirmCodeWidget;
class PrepayWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PrepayWidget(qulonglong &managerId, QString &managerName, int &level, QString &token, qulonglong &companyId, QString &companyName, double &latitude, double &longitude, QWidget *parent = 0);
    ~PrepayWidget();

private slots:
    void requestCompanyCustomer();
    void handleGetCompanyCustomerFinished();
    void handleDueEditingFinshed();
    void handleDueEditingUnFinshed();
    void handleConfirmCodePass();
    void handleConfirmCodeSkip();
    void handlePostConsumeFinished();
    void goStepOne();
    void goStepTwo();
    void goStepThree();

private:
    void initActionsForFirstStep();
    void initActionsForLastStep();
    void postConsume();

    CompanyCustomer *_companyCustomer;

    StepIndicator *_stepIndicator;
    TipsWidget *_tipsWidget;
    QToolBar   *_buttonBar;
    QAction    *_nextAction;
    QAction    *_previousAction;
    QAction    *_endAction;

    DueEditor *_dueEditor;
    PreNetEditor *_netEditor;
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
    QNetworkReply *_getCompanyCustomerReply;
    QNetworkReply *_postConsumeReply;
};

#endif

