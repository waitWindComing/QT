#include "reportwidget.h"
#include "tipswidget.h"
#include "staticscustomer.h"
#include "staticscustomermodel.h"
#include "staticsissue.h"
#include "staticsissuemodel.h"
#include "staticsaccept.h"
#include "staticsacceptmodel.h"
#include "staticstransfer.h"
#include "staticstransfermodel.h"
#include "companyaccount.h"
#include "companyaccountmodel.h"
#include "utils.h"
#include <QToolBar>
#include <QMessageBox>
#include <QCalendarWidget>
#include <QJson/Serializer>
#include <QJson/Parser>
#include <QHeaderView>
#include <qDebug>

ReportWidget::ReportWidget(qulonglong &managerId, QString &managerName, int &level, QString &token, qulonglong &companyId, QString &companyName, double &latitude, double &longitude, QWidget *parent) :
    QWidget(parent),
    _managerId(managerId),
    _managerName(managerName),
    _level(level),
    _token(token),
    _companyId(companyId),
    _companyName(companyName),
    _latitude(latitude),
    _longitude(longitude)
{
    QString title = trUtf8("统计:");
    QList<QString> tips;
    tips << trUtf8("统计一段时间内的新用户数；");
    tips << trUtf8("统计一段时间内的用户消费情况（应付、实付）；");
    tips << trUtf8("统计一段时间内优惠券发放情况，每种发行规则所发放的优惠券；");
    tips << trUtf8("统计一段时间内收券情况，包括合作商户发行的优惠券的在本商户使用");
    tips << trUtf8("统计一段时间内本商户所发券的使用情况，包括在合作商户使用");
    tips << trUtf8("统计一段时间内优惠券转让情况，包括转让次数以及通过转让优惠券的方式而产生的新用户数");
    
    _tipsWidget = new TipsWidget(title, tips, this);
    _tipsWidget->setGeometry(0, 0, 250, 600);

    QToolBar *t1 = new QToolBar(this);
    t1->setGeometry(250, 0, 300, 54);
    t1->setLayoutDirection(Qt::LeftToRight);
    _balanceLabel = new QLabel(this);
    t1->addWidget(_balanceLabel);
    QToolBar *t2 = new QToolBar(this);
    t2->setLayoutDirection(Qt::RightToLeft);
    t2->setGeometry(250 + 300, 0, 1024 - 250 - 300, 54);
    t2->setIconSize(QSize(52, 52));
    _customerAction = t2->addAction(QIcon(":/ticket/consume_stat.png"), trUtf8("用户和消费统计"));
    _issueAction = t2->addAction(QIcon(":/ticket/output_stat.png"), trUtf8("发券统计"));
    _acceptAction = t2->addAction(QIcon(":/ticket/input_stat.png"), trUtf8("收券统计"));
    _useAction = t2->addAction(QIcon(":/ticket/use_stat.png"), trUtf8("使用统计"));
    _transferAction = t2->addAction(QIcon(":/ticket/transfer_stat.png"), trUtf8("转移统计"));
    _accountAction = t2->addAction(QIcon(":/ticket/account_stat.png"), trUtf8("账户明细"));
    _backAction = t2->addAction(QIcon(":/ticket/back.png"), trUtf8("返回"));
    _exportAction = t2->addAction(QIcon(":/ticket/save.png"), trUtf8("导出"));
    connect(_customerAction, SIGNAL(triggered()), this, SLOT(requestStaticsCustomer()));
    connect(_issueAction, SIGNAL(triggered()), this, SLOT(requestStaticsIssue()));
    connect(_acceptAction, SIGNAL(triggered()), this, SLOT(requestStaticsAcceptByAccept()));
    connect(_useAction, SIGNAL(triggered()), this, SLOT(requestStaticsAcceptByIssue()));
    connect(_transferAction, SIGNAL(triggered()), this, SLOT(requestStaticsTransfer()));
    connect(_accountAction, SIGNAL(triggered()), this, SLOT(requestCompanyAccount()));
    connect(_backAction, SIGNAL(triggered()), this, SLOT(back()));
    connect(_exportAction, SIGNAL(triggered()), this, SLOT(exportToFile()));

    _startLabel = new QLabel(trUtf8("起始日期"), this);
    _startLabel->setGeometry(430, 120, 100, 40);
    _startDate = new QDateEdit(QDate::currentDate().addMonths(-1), this);
    _startDate->setCalendarPopup(true);
    QCalendarWidget *cal;
    cal = _startDate->calendarWidget();
    cal->setFixedSize(350, 280);
    _startDate->setGeometry(530, 120, 250, 40);
    _endLabel = new QLabel(trUtf8("结束日期"), this);
    _endLabel->setGeometry(430, 220, 100, 40);
    _endDate = new QDateEdit(QDate::currentDate(), this);
    _endDate->setCalendarPopup(true);
    cal = _endDate->calendarWidget();
    cal->setFixedSize(350, 280);
    _endDate->setGeometry(530, 220, 250, 40);
    _startDate->setDisplayFormat("yyyy-MM-dd");
    _endDate->setDisplayFormat("yyyy-MM-dd");
    connect(_startDate, SIGNAL(dateChanged(const QDate &)), this, SLOT(handleDateChanged(const QDate &)));
    connect(_endDate, SIGNAL(dateChanged(const QDate &)), this, SLOT(handleDateChanged(const QDate &)));

    _reportView = new QTableView(this);
    _reportView->setGeometry(250, 54, 1024 - 250, 600 - 54);
    _reportView->horizontalHeader()->setVisible(true);
    _reportView->verticalHeader()->setVisible(false);
    _currentTag = 0;
    showResult();

    _customerModel = new StaticsCustomerModel(_staticsCustomers, this);
    _issueModel = new StaticsIssueModel(_staticsIssues, this);
    _acceptModel = new StaticsAcceptModel(_staticsAccepts, StaticsAcceptModel::ByAccept, this);   // 本商户接收的券
    _useModel = new StaticsAcceptModel(_staticsAccepts, StaticsAcceptModel::ByIssue, this);       // 本商户的券的使用情况
    _transferModel = new StaticsTransferModel(_staticsTransfers, this);
    _companyAccountModel = new CompanyAccountModel(_companyAccounts, this);

    requestBalance();
}

ReportWidget::~ReportWidget()
{
    for (QList<StaticsCustomer *>::iterator i = _staticsCustomers.begin(); i!= _staticsCustomers.end(); ++i) {
        delete *i;
    }

    for (QList<StaticsIssue *>::iterator i = _staticsIssues.begin(); i!= _staticsIssues.end(); ++i) {
        delete *i;
    }
    for (QList<StaticsAccept *>::iterator i = _staticsAccepts.begin(); i!= _staticsAccepts.end(); ++i) {
        delete *i;
    }
    for (QList<StaticsTransfer *>::iterator i = _staticsTransfers.begin(); i!= _staticsTransfers.end(); ++i) {
        delete *i;
    }
    for (QList<CompanyAccount *>::iterator i = _companyAccounts.begin(); i!= _companyAccounts.end(); ++i) {
        delete *i;
    }
}

void ReportWidget::requestBalance()
{
    QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/balance")).arg(_managerId)));
    request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
    _getBalanceReply = _qnam.get(request);
    connect(_getBalanceReply, SIGNAL(finished()), this, SLOT(handleGetBalanceFinished()));
    _waitingDialog = new QProgressDialog(trUtf8("请稍后..."), QString(), 0, 0, this);
    _waitingDialog->exec();
}

void ReportWidget::handleGetBalanceFinished() {
    _waitingDialog->cancel();
    if (_getBalanceReply != NULL) {
        _getBalanceReply->close();
        _getBalanceReply->deleteLater();
        if (_getBalanceReply->error() == QNetworkReply::NoError) {
            QByteArray json = _getBalanceReply->readAll();
            QJson::Parser parser;
            bool ok;
            QVariantMap var = parser.parse (json, &ok).toMap();
            if (!ok) {
                qDebug() << "some error happend on server";
                return;
            }
            int balance = var["balance"].toInt();
            _balanceLabel->setText(QString("%1: %2%3").arg(trUtf8("您的账户当前余额")).arg(balance / 100.0, 0, 'f', 2).arg(trUtf8("元")));
        } else if (_getBalanceReply->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
        }
    }
}


void ReportWidget::showResult()
{
    switch(_currentTag) {
    case 0:
        _startLabel->show();
        _endLabel->show();
        _startDate->show();
        _endDate->show();
        _customerAction->setVisible(true);
        _issueAction->setVisible(true);
        _acceptAction->setVisible(true);
        _useAction->setVisible(true);
        _transferAction->setVisible(true);
        _accountAction->setVisible(true);
        _exportAction->setVisible(false);
        _backAction->setVisible(false);
        _reportView->hide();
        break;
    case 1:
        _startLabel->hide();
        _endLabel->hide();
        _startDate->hide();
        _endDate->hide();
        _customerAction->setVisible(false);
        _issueAction->setVisible(false);
        _acceptAction->setVisible(false);
        _useAction->setVisible(false);
        _transferAction->setVisible(false);
        _accountAction->setVisible(false);
        _exportAction->setVisible(true);
        _backAction->setVisible(true);
        _reportView->setModel(_customerModel);
        _reportView->setColumnWidth(0, 90);
        _reportView->setColumnWidth(1, 100);
        _reportView->setColumnWidth(2, 100);
        _reportView->setColumnWidth(3, 100);
        _reportView->setColumnWidth(4, 100);
        _reportView->show();
        break;
    case 2:
        _startLabel->hide();
        _endLabel->hide();
        _startDate->hide();
        _endDate->hide();
        _customerAction->setVisible(false);
        _issueAction->setVisible(false);
        _acceptAction->setVisible(false);
        _useAction->setVisible(false);
        _transferAction->setVisible(false);
        _accountAction->setVisible(false);
        _exportAction->setVisible(true);
        _backAction->setVisible(true);
        _reportView->setModel(_issueModel);
        for (int i = 0; i < _issueModel->rowCount(); ++i) {
            int j = i + 1;
            for (; j < _issueModel->rowCount(); ++j) {
                if (_issueModel->data(_issueModel->index(i, 0), Qt::DisplayRole) != _issueModel->data(_issueModel->index(j, 0), Qt::DisplayRole)) {
                    break;
                }
            }
            _reportView->setSpan(i, 0, j - i, 1);
        }
        _reportView->setColumnWidth(0, 90);
        _reportView->setColumnWidth(1, 468);
        _reportView->setColumnWidth(2, 90);
        _reportView->setColumnWidth(3, 90);
        _reportView->show();
        break;
    case 3:
        _startLabel->hide();
        _endLabel->hide();
        _startDate->hide();
        _endDate->hide();
        _customerAction->setVisible(false);
        _issueAction->setVisible(false);
        _acceptAction->setVisible(false);
        _useAction->setVisible(false);
        _transferAction->setVisible(false);
        _accountAction->setVisible(false);
        _exportAction->setVisible(true);
        _backAction->setVisible(true);
        _reportView->setModel(_acceptModel);
        for (int i = 0; i < _acceptModel->rowCount(); ++i) {
            int j = i + 1;
            for (; j < _acceptModel->rowCount(); ++j) {
                if (_acceptModel->data(_acceptModel->index(i, 0), Qt::DisplayRole) != _acceptModel->data(_acceptModel->index(j, 0), Qt::DisplayRole)) {
                    break;
                }
            }
            _reportView->setSpan(i, 0, j - i, 1);

            // for column date 
            for (int m = i; m < j; ++m) {
                int n = m + 1;
                for (; n < j; ++n) {
                    if (_acceptModel->data(_acceptModel->index(m, 1), Qt::DisplayRole) != _acceptModel->data(_acceptModel->index(n, 1), Qt::DisplayRole)) {
                        break;
                    }
                }
                _reportView->setSpan(m, 1, n - m, 1);
            }
        }
        _reportView->setColumnWidth(0, 234);
        _reportView->setColumnWidth(1, 90);
        _reportView->setColumnWidth(2, 234);
        _reportView->setColumnWidth(3, 90);
        _reportView->setColumnWidth(4, 90);
        _reportView->show();
        break;
    case 4:
        _startLabel->hide();
        _endLabel->hide();
        _startDate->hide();
        _endDate->hide();
        _customerAction->setVisible(false);
        _issueAction->setVisible(false);
        _acceptAction->setVisible(false);
        _useAction->setVisible(false);
        _transferAction->setVisible(false);
        _accountAction->setVisible(false);
        _exportAction->setVisible(true);
        _backAction->setVisible(true);
        _reportView->setModel(_useModel);
        for (int i = 0; i < _useModel->rowCount(); ++i) {
            int j = i + 1;
            for (; j < _useModel->rowCount(); ++j) {
                if (_useModel->data(_useModel->index(i, 0), Qt::DisplayRole) != _useModel->data(_useModel->index(j, 0), Qt::DisplayRole)) {
                    break;
                }
            }
            _reportView->setSpan(i, 0, j - i, 1);

            // for column date 
            for (int m = i; m < j; ++m) {
                int n = m + 1;
                for (; n < j; ++n) {
                    if (_useModel->data(_useModel->index(m, 1), Qt::DisplayRole) != _useModel->data(_useModel->index(n, 1), Qt::DisplayRole)) {
                        break;
                    }
                }
                _reportView->setSpan(m, 1, n - m, 1);
            }
        }
        _reportView->setColumnWidth(0, 234);
        _reportView->setColumnWidth(1, 90);
        _reportView->setColumnWidth(2, 234);
        _reportView->setColumnWidth(3, 90);
        _reportView->setColumnWidth(4, 90);
        _reportView->show();
        break;
    case 5:
        _startLabel->hide();
        _endLabel->hide();
        _startDate->hide();
        _endDate->hide();
        _customerAction->setVisible(false);
        _issueAction->setVisible(false);
        _acceptAction->setVisible(false);
        _useAction->setVisible(false);
        _transferAction->setVisible(false);
        _accountAction->setVisible(false);
        _exportAction->setVisible(true);
        _backAction->setVisible(true);
        _reportView->setModel(_transferModel);
        _reportView->setColumnWidth(0, 90);
        _reportView->setColumnWidth(1, 170);
        _reportView->setColumnWidth(2, 100);
        _reportView->show();
        break;
    case 6:
        _startLabel->hide();
        _endLabel->hide();
        _startDate->hide();
        _endDate->hide();
        _customerAction->setVisible(false);
        _issueAction->setVisible(false);
        _acceptAction->setVisible(false);
        _useAction->setVisible(false);
        _transferAction->setVisible(false);
        _accountAction->setVisible(false);
        _exportAction->setVisible(true);
        _backAction->setVisible(true);
        _reportView->setModel(_companyAccountModel);
        _reportView->show();
        _reportView->setColumnWidth(0, 110);
        _reportView->setColumnWidth(1, 110);
        _reportView->setColumnWidth(2, 90);
        _reportView->setColumnWidth(3, 110);
        _reportView->setColumnWidth(4, 110);
        _reportView->setColumnWidth(5, 110);
        break;
    default:
        break;
    }
}

void ReportWidget::requestStaticsCustomer()
{
    qDebug() << "ReportWidget::requestStaticsCustomer()";
    QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/staticscustomers?fy=%2&fm=%3&fd=%4&ty=%5&tm=%6&td=%7"))
         .arg(_managerId).arg(_startDate->date().year()).arg(_startDate->date().month()).arg(_startDate->date().day())
         .arg(_endDate->date().year()).arg(_endDate->date().month()).arg(_endDate->date().day())));
    request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
    _getCustomerReply = _qnam.get(request);
    connect(_getCustomerReply, SIGNAL(finished()), this, SLOT(handleGetCustomersFinished()));
    _waitingDialog = new QProgressDialog(trUtf8("请稍后..."), QString(), 0, 0, this);
    _waitingDialog->exec();
}

void ReportWidget::handleGetCustomersFinished()
{
    qDebug() << "ReportWidget::handleGetCustomersFinished()";
    _waitingDialog->cancel();
    for (QList<StaticsCustomer *>::iterator i = _staticsCustomers.begin(); i!= _staticsCustomers.end(); ++i) {
        delete *i;
    }
    _staticsCustomers.clear();

    if (_getCustomerReply != NULL) {
        _getCustomerReply->close();
        _getCustomerReply->deleteLater();
        if (_getCustomerReply->error() == QNetworkReply::NoError) {
            QByteArray json = _getCustomerReply->readAll();
            QJson::Parser parser;
            bool ok;
            QVariantList customers = parser.parse (json, &ok).toList();
            if (!ok) {
                qDebug() << "some error happend on server";
                return;
            }
            for (QVariantList::iterator i = customers.begin(); i != customers.end(); ++i) {
                _staticsCustomers.append(StaticsCustomer::fromJson(*i));
            }
            if (_staticsCustomers.size() == 0) {
                QMessageBox::information(this, trUtf8("结果为空"), trUtf8("没有查询到任何结果！"));
            }
            _currentTag = 1;
            showResult();
        } else if (_getCustomerReply->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
            QMessageBox::critical(this, trUtf8("认证失败"), trUtf8("请退出系统重新认证！"));
        }
    }
}

void ReportWidget::requestStaticsIssue()
{
    qDebug() << "ReportWidget::requestStaticsIssue()";
    QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/staticsissues?fy=%2&fm=%3&fd=%4&ty=%5&tm=%6&td=%7"))
         .arg(_managerId).arg(_startDate->date().year()).arg(_startDate->date().month()).arg(_startDate->date().day())
         .arg(_endDate->date().year()).arg(_endDate->date().month()).arg(_endDate->date().day())));
    request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
    _getIssueReply = _qnam.get(request);
    connect(_getIssueReply, SIGNAL(finished()), this, SLOT(handleGetIssuesFinished()));
    _waitingDialog = new QProgressDialog(trUtf8("请稍后..."), QString(), 0, 0, this);
    _waitingDialog->exec();
}

void ReportWidget::handleGetIssuesFinished()
{
    qDebug() << "ReportWidget::handleGetIssuesFinished()";
    _waitingDialog->cancel();
    for (QList<StaticsIssue *>::iterator i = _staticsIssues.begin(); i!= _staticsIssues.end(); ++i) {
        delete *i;
    }
    _staticsIssues.clear();

    if (_getIssueReply != NULL) {
        _getIssueReply->close();
        _getIssueReply->deleteLater();
        if (_getIssueReply->error() == QNetworkReply::NoError) {
            QByteArray json = _getIssueReply->readAll();
            QJson::Parser parser;
            bool ok;
            QVariantList issues = parser.parse (json, &ok).toList();
            if (!ok) {
                qDebug() << "some error happend on server";
                return;
            }
            for (QVariantList::iterator i = issues.begin(); i != issues.end(); ++i) {
                _staticsIssues.append(StaticsIssue::fromJson(*i));
            }
            if (_staticsIssues.size() == 0) {
                QMessageBox::information(this, trUtf8("结果为空"), trUtf8("没有查询到任何结果！"));
            }
            _currentTag = 2;
            showResult();
        } else if (_getIssueReply->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
            QMessageBox::critical(this, trUtf8("认证失败"), trUtf8("请退出系统重新认证！"));
        }
    }
}

void ReportWidget::requestStaticsAcceptByAccept()
{
    qDebug() << "ReportWidget::requestStaticsAcceptByAccept()";
    QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/staticsacceptsbyaccept?fy=%2&fm=%3&fd=%4&ty=%5&tm=%6&td=%7"))
         .arg(_managerId).arg(_startDate->date().year()).arg(_startDate->date().month()).arg(_startDate->date().day())
         .arg(_endDate->date().year()).arg(_endDate->date().month()).arg(_endDate->date().day())));
    request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
    _getAcceptReply = _qnam.get(request);
    connect(_getAcceptReply, SIGNAL(finished()), this, SLOT(handleGetAcceptsByAcceptFinished()));
    _waitingDialog = new QProgressDialog(trUtf8("请稍后..."), QString(), 0, 0, this);
    _waitingDialog->exec();
}

void ReportWidget::handleGetAcceptsByAcceptFinished()
{
    qDebug() << "ReportWidget::handleGetAcceptsByAcceptFinished()";
    _waitingDialog->cancel();
    for (QList<StaticsAccept *>::iterator i = _staticsAccepts.begin(); i!= _staticsAccepts.end(); ++i) {
        delete *i;
    }
    _staticsAccepts.clear();

    if (_getAcceptReply != NULL) {
        _getAcceptReply->close();
        _getAcceptReply->deleteLater();
        if (_getAcceptReply->error() == QNetworkReply::NoError) {
            QByteArray json = _getAcceptReply->readAll();
            QJson::Parser parser;
            bool ok;
            QVariantList accepts = parser.parse (json, &ok).toList();
            if (!ok) {
                qDebug() << "some error happend on server";
                return;
            }
            for (QVariantList::iterator i = accepts.begin(); i != accepts.end(); ++i) {
                _staticsAccepts.append(StaticsAccept::fromJson(*i));
            }
            if (_staticsAccepts.size() == 0) {
                QMessageBox::information(this, trUtf8("结果为空"), trUtf8("没有查询到任何结果！"));
            }
            _currentTag = 3;
            showResult();
        } else if (_getAcceptReply->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
            QMessageBox::critical(this, trUtf8("认证失败"), trUtf8("请退出系统重新认证！"));
        }
    }
}

void ReportWidget::requestStaticsAcceptByIssue()
{
    qDebug() << "ReportWidget::requestStaticsAcceptByIssue()";
    QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/staticsacceptsbyissue?fy=%2&fm=%3&fd=%4&ty=%5&tm=%6&td=%7"))
         .arg(_managerId).arg(_startDate->date().year()).arg(_startDate->date().month()).arg(_startDate->date().day())
         .arg(_endDate->date().year()).arg(_endDate->date().month()).arg(_endDate->date().day())));
    request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
    _getAcceptReply = _qnam.get(request);
    connect(_getAcceptReply, SIGNAL(finished()), this, SLOT(handleGetAcceptsByIssueFinished()));
    _waitingDialog = new QProgressDialog(trUtf8("请稍后..."), QString(), 0, 0, this);
    _waitingDialog->exec();
}

void ReportWidget::handleGetAcceptsByIssueFinished()
{
    qDebug() << "ReportWidget::handleGetAcceptsByIssueFinished()";
    _waitingDialog->cancel();
    for (QList<StaticsAccept *>::iterator i = _staticsAccepts.begin(); i!= _staticsAccepts.end(); ++i) {
        delete *i;
    }
    _staticsAccepts.clear();

    if (_getAcceptReply != NULL) {
        _getAcceptReply->close();
        _getAcceptReply->deleteLater();
        if (_getAcceptReply->error() == QNetworkReply::NoError) {
            QByteArray json = _getAcceptReply->readAll();
            QJson::Parser parser;
            bool ok;
            QVariantList accepts = parser.parse (json, &ok).toList();
            if (!ok) {
                qDebug() << "some error happend on server";
                return;
            }
            for (QVariantList::iterator i = accepts.begin(); i != accepts.end(); ++i) {
                _staticsAccepts.append(StaticsAccept::fromJson(*i));
            }
            if (_staticsAccepts.size() == 0) {
                QMessageBox::information(this, trUtf8("结果为空"), trUtf8("没有查询到任何结果！"));
            }
            _currentTag = 4;
            showResult();
        } else if (_getAcceptReply->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
            QMessageBox::critical(this, trUtf8("认证失败"), trUtf8("请退出系统重新认证！"));
        }
    }
}

void ReportWidget::requestStaticsTransfer()
{
    qDebug() << "ReportWidget::requestStaticsTransfer()";
    QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/staticstransfers?fy=%2&fm=%3&fd=%4&ty=%5&tm=%6&td=%7"))
         .arg(_managerId).arg(_startDate->date().year()).arg(_startDate->date().month()).arg(_startDate->date().day())
         .arg(_endDate->date().year()).arg(_endDate->date().month()).arg(_endDate->date().day())));
    request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
    _getTransferReply = _qnam.get(request);
    connect(_getTransferReply, SIGNAL(finished()), this, SLOT(handleGetTransfersFinished()));
    _waitingDialog = new QProgressDialog(trUtf8("请稍后..."), QString(), 0, 0, this);
    _waitingDialog->exec();
}

void ReportWidget::handleGetTransfersFinished()
{
    qDebug() << "ReportWidget::handleGetTransfersFinished()";
    _waitingDialog->cancel();
    for (QList<StaticsTransfer *>::iterator i = _staticsTransfers.begin(); i!= _staticsTransfers.end(); ++i) {
        delete *i;
    }
    _staticsTransfers.clear();

    if (_getTransferReply != NULL) {
        _getTransferReply->close();
        _getTransferReply->deleteLater();
        if (_getTransferReply->error() == QNetworkReply::NoError) {
            QByteArray json = _getTransferReply->readAll();
            QJson::Parser parser;
            bool ok;
            QVariantList transfers = parser.parse (json, &ok).toList();
            if (!ok) {
                qDebug() << "some error happend on server";
                return;
            }
            for (QVariantList::iterator i = transfers.begin(); i != transfers.end(); ++i) {
                _staticsTransfers.append(StaticsTransfer::fromJson(*i));
            }
            if (_staticsTransfers.size() == 0) {
                QMessageBox::information(this, trUtf8("结果为空"), trUtf8("没有查询到任何结果！"));
            }
            _currentTag = 5;
            showResult();
        } else if (_getTransferReply->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
            QMessageBox::critical(this, trUtf8("认证失败"), trUtf8("请退出系统重新认证！"));
        }
    }
}

void ReportWidget::requestCompanyAccount()
{
    qDebug() << "ReportWidget::requestCompanyAccount()";
    QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/companyaccounts?fy=%2&fm=%3&fd=%4&ty=%5&tm=%6&td=%7"))
         .arg(_managerId).arg(_startDate->date().year()).arg(_startDate->date().month()).arg(_startDate->date().day())
         .arg(_endDate->date().year()).arg(_endDate->date().month()).arg(_endDate->date().day())));
    request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
    _getCompanyAccountReply = _qnam.get(request);
    connect(_getCompanyAccountReply, SIGNAL(finished()), this, SLOT(handleGetCompanyAccountsFinished()));
    _waitingDialog = new QProgressDialog(trUtf8("请稍后..."), QString(), 0, 0, this);
    _waitingDialog->exec();
}

void ReportWidget::handleGetCompanyAccountsFinished()
{
    qDebug() << "ReportWidget::handleGetCompanyAccountsFinished()";
    _waitingDialog->cancel();
    for (QList<CompanyAccount *>::iterator i = _companyAccounts.begin(); i!= _companyAccounts.end(); ++i) {
        delete *i;
    }
    _companyAccounts.clear();

    if (_getCompanyAccountReply != NULL) {
        _getCompanyAccountReply->close();
        _getCompanyAccountReply->deleteLater();
        if (_getCompanyAccountReply->error() == QNetworkReply::NoError) {
            QByteArray json = _getCompanyAccountReply->readAll();
            QJson::Parser parser;
            bool ok;
            QVariantList accepts = parser.parse (json, &ok).toList();
            if (!ok) {
                qDebug() << "some error happend on server";
                return;
            }
            for (QVariantList::iterator i = accepts.begin(); i != accepts.end(); ++i) {
                _companyAccounts.append(CompanyAccount::fromJson(*i));
            }
            if (_companyAccounts.size() == 0) {
                QMessageBox::information(this, trUtf8("结果为空"), trUtf8("没有查询到任何结果！"));
            }
            _currentTag = 6;
            showResult();
        } else if (_getCompanyAccountReply->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
            QMessageBox::critical(this, trUtf8("认证失败"), trUtf8("请退出系统重新认证！"));
        }
    }
}

void ReportWidget::exportToFile()
{
    QString fileName;
    switch(_currentTag) {
    case 1:
        fileName = _customerModel->exportToFile();
        break;
    case 2:
        fileName = _issueModel->exportToFile();
        break;
    case 3:
        fileName = _acceptModel->exportToFile();
        break;
    case 4:
        fileName = _useModel->exportToFile();
        break;
    case 5:
        fileName = _transferModel->exportToFile();
        break;
    case 6:
        fileName = _companyAccountModel->exportToFile();
        break;
    default:
        break;
    }
    QMessageBox::information(this, trUtf8("导出成功"), fileName);
}

void ReportWidget::back()
{
    _currentTag = 0;
    showResult();
}

void ReportWidget::handleDateChanged(const QDate &date)
{
    if (sender() == _startDate) {
        if (date.addDays(31) < _endDate->date()) {
            _endDate->setDate(date.addDays(31));
        }
    } else {
        if (date.addDays(-31) > _startDate->date()) {
            _startDate->setDate(date.addDays(-31));
        }
    }
}

