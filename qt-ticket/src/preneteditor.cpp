#include "preneteditor.h"
#include "companycustomer.h"
#include "customer.h"
#include <QLabel>
#include <qDebug>

PreNetEditor::PreNetEditor(const qulonglong &managerId, const QString &managerName, const int &level, const QString &token, const qulonglong &companyId, const QString &companyName, const double &latitude, const double &longitude, qlonglong due, CompanyCustomer *companyCustomer, QWidget *parent) :
    _managerId(managerId),
    _managerName(managerName),
    _level(level),
    _token(token),
    _companyId(companyId),
    _companyName(companyName),
    _latitude(latitude),
    _longitude(longitude),
    _due(due),
    _companyCustomer(companyCustomer),
    QWidget(parent)
{
    QLabel *customerLabel = new QLabel(trUtf8("用户号码：") + companyCustomer->getCustomer()->getName(), this);
    customerLabel->setGeometry(40, 100, 380, 30);
    customerLabel->setFont(QFont("Timers", 22));
    QLabel *balanceLabel = new QLabel(QString("%1%2%3").arg(trUtf8("账户余额：")).arg(companyCustomer->getBalance() / 100.0, 0, 'f', 2).arg(trUtf8("元")), this);
    balanceLabel->setGeometry(40, 140, 380, 30);
    balanceLabel->setFont(QFont("Timers", 22));
    QLabel *discountLabel = new QLabel(QString("%1%2%3").arg(trUtf8("账户折扣：")).arg(companyCustomer->getDiscount()).arg(trUtf8("％")), this);
    discountLabel->setGeometry(40, 180, 380, 30);
    discountLabel->setFont(QFont("Timers", 22));
    QLabel *dueLabel = new QLabel(QString("%1%2%3").arg(trUtf8("应付：")).arg(due / 100.0, 0, 'f', 2).arg(trUtf8("元")), this);
    dueLabel->setGeometry(40, 220, 380, 30);
    dueLabel->setFont(QFont("Timers", 22));
    float net = due * companyCustomer->getDiscount() / 100.0;
    QLabel *netLabel = new QLabel(QString("%1%2%3").arg(trUtf8("实付：")).arg(net / 100.0, 0, 'f', 2).arg(trUtf8("元")), this);
    netLabel->setGeometry(40, 260, 380, 30);
    netLabel->setFont(QFont("Timers", 22));
    if (companyCustomer->getBalance() < net) {
        QLabel *warningLabel = new QLabel(QString("<font color=red size=60>%1</font>").arg(trUtf8("余额不足！")), this);
        warningLabel->setGeometry(40, 300, 380, 60);
    }
}

PreNetEditor::~PreNetEditor()
{
}


