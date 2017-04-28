#include "paywidget.h"
#include "stepindicator.h"
#include "tipswidget.h"
#include "dueeditor.h"
#include "reduceeditor.h"
#include "neteditor.h"
#include "confirmcodewidget.h"
#include "issuedticketdlg.h"
#include "issueeditor.h"
#include "companycustomer.h"
#include "customer.h"
#include "consume.h"
#include "ticket.h"
#include "userule.h"
#include "issuerule.h"
#include "utils.h"
#include <QToolBar>
#include <QMessageBox>
#include <QMap>
#include <QJson/Serializer>
#include <QJson/Parser>
#include <qDebug>

PayWidget::PayWidget(qulonglong &managerId, QString &managerName, int &level, QString &token, qulonglong &companyId, QString &companyName, double &latitude, double &longitude, QWidget *parent) :
    QWidget(parent),
    _managerId(managerId),
    _managerName(managerName),
    _level(level),
    _token(token),
    _companyId(companyId),
    _companyName(companyName),
    _latitude(latitude),
    _longitude(longitude),
    _skipStepTwo(false),
    _stepIndicator(NULL),
    _netEditor(NULL),
    _issueEditor(NULL),
    _customer(NULL),
    _consume(NULL)
{
    QString title = trUtf8("消费者本次消费:");
    QList<QString> tips;
    tips << trUtf8("输入消费者的手机号码；");
    tips << trUtf8("本次的消费额度（应付）；");
    tips << trUtf8("若消费者为新用户，系统将会自动替该用户注册，并将注册信息通过短信发送到用户到手机；");
    
    _tipsWidget = new TipsWidget(title, tips, this);
    _tipsWidget->setGeometry(0, 0, 250, 540);

    createStepIndicatorWithStepTwo();

    _infoBar = new QToolBar(this);
    _infoBar->setLayoutDirection(Qt::LeftToRight);
    _payInfo = new QLabel(this);
    _infoBar->addWidget(_payInfo);
    _buttonBar = new QToolBar(this);
    _buttonBar->setLayoutDirection(Qt::RightToLeft);
    _infoBar->setGeometry(250, 540 - 54, 500, 54);
    _buttonBar->setGeometry(250 + 500, 540 - 54, 1024 - 250 - 500, 54);
    _buttonBar->setIconSize(QSize(52, 52));
    initActionsForFirstStep();

    _dueEditor = new DueEditor(_managerId, _managerName, _level, _token, _companyId, _companyName, _latitude, _longitude, this);
    _dueEditor->setGeometry(250, 0, 1024 - 250, 540 - 54);
    connect(_dueEditor, SIGNAL(editingFinished()), this, SLOT(handleDueEditingFinshed()));
    connect(_dueEditor, SIGNAL(editingUnFinished()), this, SLOT(handleDueEditingUnFinshed()));

    _reduceEditor = new ReduceEditor(_managerId, _managerName, _level, _token, _companyId, _companyName, _latitude, _longitude, _tickets, _useRules, this);
    _reduceEditor->setGeometry(250, 0, 1024 - 250, 540 - 54);

    requestUseRules();
    requestIssueRules();

    goStepOne();
}

PayWidget::~PayWidget()
{
    if (_customer) delete _customer;
    if (_consume) delete _consume;
    for (QList<Ticket *>::iterator i = _tickets.begin(); i!= _tickets.end(); ++i) {
        delete *i;
    }
    for (QList<UseRule *>::iterator i = _useRules.begin(); i!= _useRules.end(); ++i) {
        delete *i;
    }
    if (_stepIndicator != NULL) delete _stepIndicator;
}

void PayWidget::handleDueEditingFinshed()
{
    _nextAction->setEnabled(true);
    _nextAction->disconnect();
    connect(_nextAction, SIGNAL(triggered()), this, SLOT(requestCustomer()));
}

void PayWidget::handleDueEditingUnFinshed()
{
    _nextAction->setEnabled(false);
    _nextAction->disconnect();
}

void PayWidget::goStepOne()
{
    qDebug() << "goStepOne()";
    if (sender() == _previousAction) {
        _nextAction->setEnabled(true);
        _nextAction->disconnect();
        connect(_nextAction, SIGNAL(triggered()), this, SLOT(requestCustomer()));
    } else {
        if (_customer) {
            delete _customer;
            _customer = NULL;
        }
        if (_consume) {
            delete _consume;
            _consume = NULL;
        }
        for (QList<Ticket *>::iterator i = _tickets.begin(); i!= _tickets.end(); ++i) {
            delete *i;
        }
        _tickets.clear();

        _dueEditor->clear();
        _nextAction->setEnabled(false);
    }
    _previousAction->setEnabled(false);
    if (_netEditor != NULL) {
        delete _netEditor;
        _netEditor = NULL;
    }
    if (_issueEditor != NULL) {
        delete _issueEditor;
        _issueEditor = NULL;
    }
    _reduceEditor->hide();
    _dueEditor->show();
    QString currentStep = trUtf8("应付");
    _stepIndicator->setCurrentStep(currentStep);
}

void PayWidget::goStepTwo()
{
    qDebug() << "goStepTwo()";
    if (sender() == _previousAction) {
        delete _netEditor;
        _netEditor = NULL;
    }
    _nextAction->setEnabled(true);
    _nextAction->disconnect();
    connect(_nextAction, SIGNAL(triggered()), this, SLOT(goStepThree()));

    _previousAction->setEnabled(true);
    _previousAction->disconnect();
    connect(_previousAction, SIGNAL(triggered()), this, SLOT(goStepOne()));

    _dueEditor->hide();
    _reduceEditor->show();
    QString currentStep = trUtf8("优惠");
    _stepIndicator->setCurrentStep(currentStep);

    QString title = trUtf8("选择消费者可用到优惠券:");
    QList<QString> tips;
    tips << trUtf8("优惠券有折扣券和代金券两种，这两种优惠券不能混合同时使用");
    tips << trUtf8("折扣券只能使用一张");
    tips << trUtf8("代金券可以同时使用多张");
    
    delete _tipsWidget;
    _tipsWidget = new TipsWidget(title, tips, this);
    _tipsWidget->setGeometry(0, 0, 250, 540);
    _tipsWidget->show();
}

void PayWidget::goStepThree()
{
    qDebug() << "goStepThree()";
    _nextAction->setEnabled(true);
    _nextAction->disconnect();
    connect(_nextAction, SIGNAL(triggered()), this, SLOT(goStepFour()));

    _previousAction->setEnabled(true);
    _previousAction->disconnect();
    if (_skipStepTwo) {
        connect(_previousAction, SIGNAL(triggered()), this, SLOT(goStepOne()));
    } else {
        connect(_previousAction, SIGNAL(triggered()), this, SLOT(goStepTwo()));
    }

    _dueEditor->hide();
    _reduceEditor->hide();
    QList<Ticket *> selectedTickets = getSelectedTickets();
    QList<UseRule *> candidateUseRules = getCandidateUseRules();
    _netEditor = new NetEditor(_managerId, _managerName, _level, _token, _companyId, _companyName, _latitude, _longitude, selectedTickets, candidateUseRules, _dueEditor->getDue(), this);
    connect(_netEditor, SIGNAL(summaryUpdated(QString &)), this, SLOT(handleSummaryUpdated(QString &)));
    _netEditor->setGeometry(250, 0, 1024 - 250, 540 - 54);
    _netEditor->show();

    QString currentStep = trUtf8("实付");
    _stepIndicator->setCurrentStep(currentStep);

    QString title = trUtf8("计算消费者优惠额度和实付额度:");
    QList<QString> tips;
    tips << trUtf8("若定义了多条使用规则，则系统会列出这些使用规则，需要选择一条使用规则");
    tips << trUtf8("系统会根据选择到使用规则计算当前到优惠额度");
    tips << trUtf8("若是代金券，系统会更新代金券抵扣后的剩余可抵扣额度");
    tips << trUtf8("若是一次性折扣券，本次使用完后将不能再次使用");
    tips << trUtf8("使用优惠券需要用户提供3为确认码，否则不能使用。用户的3位确认码在用户首次注册时会通过短信发送到用户的手机上，也可以通过APP查看");
    
    delete _tipsWidget;
    _tipsWidget = new TipsWidget(title, tips, this);
    _tipsWidget->setGeometry(0, 0, 250, 540);
    _tipsWidget->show();
}

void PayWidget::goStepFour()
{
    qDebug() << "goStepFour()";
    QString title = trUtf8("发优惠券：");
    QList<QString> tips;
    tips << trUtf8("系统会列出发放规则，需要选择一条发放规则");
    tips << trUtf8("系统会根据当前用户实付金额和所选的发放规则计算出发给用户的优惠券优惠券");
    tips << trUtf8("用户可以通过APP查看收到的优惠券");
    
    delete _tipsWidget;
    _tipsWidget = new TipsWidget(title, tips, this);
    _tipsWidget->setGeometry(0, 0, 250, 540);
    _tipsWidget->show();

    _netPaid = _netEditor->getNet();
    if (_customer == NULL) {
        postCustomer();
    } else {
        QList<Ticket *> selectedTickets = getSelectedTickets();
        if (!selectedTickets.isEmpty()) {
            _nextAction->setEnabled(false);
            _previousAction->setEnabled(false);
            _netEditor->setEnabled(false);
            _confirmCodeWidget = new ConfirmCodeWidget(_managerId, _token, _customer->getCustomerId(), this);
            _confirmCodeWidget->setGeometry(780, 150, 240, 400);
            connect(_confirmCodeWidget, SIGNAL(pass()), this, SLOT(handleConfirmCodePass()));
            connect(_confirmCodeWidget, SIGNAL(skip()), this, SLOT(handleConfirmCodeSkip()));
            _confirmCodeWidget->show();
            _confirmCodeWidget->focus();
        } else {
            postConsume();
        }
    }
}

void PayWidget::handleConfirmCodePass()
{
    qDebug() << "PayWidget::handleConfirmCodePass()";
    postConsume();
    _confirmCodeWidget->close();
    _confirmCodeWidget->deleteLater();
}

void PayWidget::handleConfirmCodeSkip()
{
    qDebug() << "PayWidget::handleConfirmCodeSkip()";
    _netPaid = _dueEditor->getDue();
    postConsume();
    _confirmCodeWidget->close();
    _confirmCodeWidget->deleteLater();
}

void PayWidget::requestUseRules()
{
    qDebug() << "PayWidget::requestUseRules()";
    QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/useRules")).arg(_managerId)));
    request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
    _useRulesReply = _qnam.get(request);
    connect(_useRulesReply, SIGNAL(finished()), this, SLOT(handleGetUseRulesFinished()));
}

void PayWidget::handleGetUseRulesFinished()
{
    qDebug() << "PayWidget::handleGetUseRulesFinished()";
    if (_useRulesReply != NULL) {
        _useRulesReply->close();
        _useRulesReply->deleteLater();
        if (_useRulesReply->error() == QNetworkReply::NoError) {
            QByteArray json = _useRulesReply->readAll();
            QJson::Parser parser;
            bool ok;
            QVariantList rules = parser.parse (json, &ok).toList();
            if (!ok) {
                qDebug() << "some error happend on server";
                return;
            }
            for (QVariantList::iterator i = rules.begin(); i != rules.end(); ++i) {
                _useRules.append(UseRule::fromJson(*i));
            }
        } else if (_useRulesReply->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
            QMessageBox::critical(this, trUtf8("认证失败"), trUtf8("请退出系统重新认证！"));
        }
    }
}

void PayWidget::requestIssueRules()
{
    QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/issueRules")).arg(_managerId)));
    request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
    _issueRulesReply = _qnam.get(request);
    connect(_issueRulesReply, SIGNAL(finished()), this, SLOT(handleGetIssueRulesFinished()));
}


void PayWidget::handleGetIssueRulesFinished() {
    if (_issueRulesReply != NULL) {
        _issueRulesReply->close();
        _issueRulesReply->deleteLater();

        if (_issueRulesReply->error() == QNetworkReply::NoError) {
            QByteArray json = _issueRulesReply->readAll();
            QJson::Parser parser;
            bool ok;
            QVariantList rules = parser.parse (json, &ok).toList();
            if (!ok) {
                qDebug() << "some error happend on server";
                return;
            }
            for (QVariantList::iterator it = rules.begin(); it != rules.end(); ++it) {
                _issueRules.append(IssueRule::fromJson(*it));
            }
        } else if (_issueRulesReply->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
            QMessageBox::critical(this, trUtf8("认证失败"), trUtf8("请退出系统重新认证！"));
        }
    }
}

void PayWidget::requestCustomer()
{
    qDebug() << "PayWidget::requestCustomer()";
    _nextAction->setEnabled(false);
    _previousAction->setEnabled(false);
    if (_customer != NULL) {
        delete _customer;
        _customer = NULL;
    }
    for (QList<Ticket *>::iterator i = _tickets.begin(); i!= _tickets.end(); ++i) {
        delete *i;
    }
    _tickets.clear();
    QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/customers?customerName=%2")).arg(_managerId).arg(_dueEditor->getCustomer())));
    request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
    _getCustomerReply = _qnam.get(request);
    connect(_getCustomerReply, SIGNAL(finished()), this, SLOT(handleGetCustomerFinished()));
}

void PayWidget::handleGetCustomerFinished()
{
    qDebug() << "PayWidget::handleGetCustomerFinished()";
    _nextAction->setEnabled(true);
    _previousAction->setEnabled(true);
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
                _customer = Customer::fromJson(*i);
                break;   // we will have only one
            }

            if (_customer != NULL) {
                requestTickets();
            } else {
                _skipStepTwo = true;
                createStepIndicatorWithoutStepTwo();
                goStepThree();
            }
        } else if (_getCustomerReply->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
            QMessageBox::critical(this, trUtf8("认证失败"), trUtf8("请退出系统重新认证！"));
        }
    }
}

void PayWidget::requestTickets()
{
    qDebug() << "PayWidget::requestTickets()";
    for (QList<Ticket *>::iterator i = _tickets.begin(); i!= _tickets.end(); ++i) {
        delete *i;
    }
    _tickets.clear();

    QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/customers/%2/tickets?state=valid&validCompany=me")).arg(_managerId).arg(_customer->getCustomerId())));
    request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
    _ticketsReply = _qnam.get(request);
    connect(_ticketsReply, SIGNAL(finished()), this, SLOT(handleGetTicketsFinished()));
}

void PayWidget::handleGetTicketsFinished()
{
    qDebug() << "PayWidget::handleGetTicketsFinished()";
    if (_ticketsReply != NULL) {
        _ticketsReply->close();
        _ticketsReply->deleteLater();
        if (_ticketsReply->error() == QNetworkReply::NoError) {
            QByteArray json = _ticketsReply->readAll();
            QJson::Parser parser;
            bool ok;
            QVariantList tickets = parser.parse (json, &ok).toList();
            if (!ok) {
                qDebug() << "some error happend on server";
                return;
            }
            for (QVariantList::iterator i = tickets.begin(); i != tickets.end(); ++i) {
                Ticket *ticket = Ticket::fromJson(*i);
                _tickets.append(ticket);
            }
        } else if (_ticketsReply->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
            QMessageBox::critical(this, trUtf8("认证失败"), trUtf8("请退出系统重新认证！"));
        } else if (_ticketsReply->error() == QNetworkReply::ContentOperationNotPermittedError) {
            qDebug() << "low balance!";
            QMessageBox::warning(this, trUtf8("余额不足"), trUtf8("因余额不足而导致操作失败，请充值！"));
        }
    }
    if (_tickets.size() == 0) {
        _skipStepTwo = true;
        createStepIndicatorWithoutStepTwo();
        goStepThree();
    } else {
        _skipStepTwo = false;
        createStepIndicatorWithStepTwo();
        goStepTwo();
    }
}

void PayWidget::createStepIndicatorWithStepTwo()
{
    if (_stepIndicator != NULL) delete _stepIndicator;
    QList<QString> steps;
    steps << trUtf8("应付") << trUtf8("优惠") << trUtf8("实付") << trUtf8("发优惠券");
    QString currentStep = trUtf8("应付");
    _stepIndicator = new StepIndicator(steps, currentStep, this);
    _stepIndicator->setGeometry(0, 540, 1024, 60);
    _stepIndicator->show();
}

void PayWidget::createStepIndicatorWithoutStepTwo()
{
    if (_stepIndicator != NULL) delete _stepIndicator;
    QList<QString> steps;
    steps << trUtf8("应付") << trUtf8("实付") << trUtf8("发优惠券");
    QString currentStep = trUtf8("应付");
    _stepIndicator = new StepIndicator(steps, currentStep, this);
    _stepIndicator->setGeometry(0, 540, 1024, 60);
    _stepIndicator->show();
}

QList<Ticket *> PayWidget::getSelectedTickets()
{
    QList<Ticket *> selectedTickets;
    for (QList<Ticket *>::iterator i = _tickets.begin(); i != _tickets.end(); ++i) {
        if ((*i)->getCheckState() == Qt::Checked) {
            selectedTickets.append(*i);
        }
    }
    return selectedTickets;
}

QList<UseRule *> PayWidget::getCandidateUseRules()
{
    QList<UseRule *> candidateUseRules;
    Ticket *selectedTicket = NULL;
    for (QList<Ticket *>::iterator i = _tickets.begin(); i != _tickets.end(); ++i) {
        if ((*i)->getCheckState() == Qt::Checked) {
            selectedTicket = *i;
            break;
        }
    }
    if (selectedTicket != NULL && Ticket::Groupon != selectedTicket->getType()) {
        for (QList<UseRule *>::iterator i = _useRules.begin(); i != _useRules.end(); ++i) {
            QList<Company> issuedByCompanies = (*i)->getIssuedByCompanies();
            if (issuedByCompanies.size() == 0) {    // 自身
                if (selectedTicket->getCompany().getCompanyId() == _companyId
                  && ((Ticket::Discount == selectedTicket->getType() && UseRule::Discount == (*i)->getType())
                      || (Ticket::Deduction == selectedTicket->getType() && UseRule::Deduction == (*i)->getType()))) {
                    candidateUseRules.append(*i);
                }
            } else {
                if (issuedByCompanies.contains(selectedTicket->getCompany())
                  && ((Ticket::Discount == selectedTicket->getType() && UseRule::Discount == (*i)->getType())
                      || (Ticket::Deduction == selectedTicket->getType() && UseRule::Deduction == (*i)->getType()))) {
                    candidateUseRules.append(*i);
                }
            }
        }
    }
    for (QList<UseRule *>::iterator i = candidateUseRules.begin(); i != candidateUseRules.end(); ++i) {
        (*i)->setCheckState(Qt::Unchecked);
    }
    if (!candidateUseRules.isEmpty()) {
        candidateUseRules.first()->setCheckState(Qt::Checked);
    }
    return candidateUseRules;
}

void PayWidget::initActionsForFirstStep()
{
    _infoBar->setGeometry(250, 540 - 54, 500, 54);
    _buttonBar->setGeometry(250 + 500, 540 - 54, 1024 - 250 - 500, 54);
    _buttonBar->setIconSize(QSize(50, 50));
    _buttonBar->clear();
    _nextAction = _buttonBar->addAction(QIcon(":/ticket/next.png"), trUtf8("下一步"));
    _buttonBar->addSeparator();
    _previousAction = _buttonBar->addAction(QIcon(":/ticket/previous.png"), trUtf8("上一步"));
    _payInfo->clear();
}

void PayWidget::initActionsForLastStep()
{
    _infoBar->setGeometry(250, 540 - 54, 300, 54);
    _buttonBar->setGeometry(250 + 300, 540 - 54, 1024 - 250 - 300, 54);
    _buttonBar->setIconSize(QSize(133, 28));
    _buttonBar->clear();
    _issueAction = _buttonBar->addAction(QIcon(":/ticket/issue.png"), trUtf8("发券"));
    _buttonBar->addSeparator();
    _cancelAction = _buttonBar->addAction(QIcon(":/ticket/cancel.png"), trUtf8("取消发券"));
}

void PayWidget::cancel()
{
    initActionsForFirstStep();
    goStepOne();
}

void PayWidget::postTicket()
{
    qDebug() << "PayWidget::postTicket";
    IssueRule *selectedRule = _issueEditor->getSelectedIssueRule();
    if (selectedRule == NULL) {
        initActionsForFirstStep();
        goStepOne();
        return;
    }

    QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/companyCustomers/%2/tickets")).arg(_managerId).arg(_consume->getCompanyCustomer()->getCompanyCustomerId())));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
    QVariantMap variant;
    variant.insert("consumeId", _consume->getConsumeId());
    variant.insert("ruleId", selectedRule->getRuleId());
    bool ok;
    QJson::Serializer serializer;
    QByteArray data = serializer.serialize(variant, &ok);
    Q_ASSERT(ok);
    _postTicketReply = _qnam.post(request, data);
    connect(_postTicketReply, SIGNAL(finished()), this, SLOT(handlePostTicketFinished()));
}

void PayWidget::handlePostTicketFinished()
{
    qDebug() << "PayWidget::handlePostTicketFinished()";
    if (_postTicketReply != NULL) {
        _postTicketReply->close();
        _postTicketReply->deleteLater();
        if (_postTicketReply->error() == QNetworkReply::NoError) {
            QByteArray json = _postTicketReply->readAll();
            QJson::Parser parser;
            bool ok;
            QVariantMap ticket = parser.parse (json, &ok).toMap();
            if (!ok) {
                qDebug() << "some error happend on server";
                return;
            }
            Ticket *issueTicket = Ticket::fromJson(ticket);
            if (issueTicket != NULL) {
                IssuedTicketDlg dlg(issueTicket, this);
                dlg.exec();
                delete issueTicket;
            }
            initActionsForFirstStep();
            goStepOne();
        } else if (_postTicketReply->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
            QMessageBox::critical(this, trUtf8("认证失败"), trUtf8("请退出系统重新认证！"));
        }
    }
}

void PayWidget::postCustomer()
{
    qDebug() << "PayWidget::postCustomer";

    _nextAction->setEnabled(false);
    _previousAction->setEnabled(false);
    if (_customer != NULL) delete _customer;

    QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/customers")).arg(_managerId)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
    QVariantMap variant;
    variant.insert("name", _dueEditor->getCustomer());
    bool ok;
    QJson::Serializer serializer;
    QByteArray data = serializer.serialize(variant, &ok);
    Q_ASSERT(ok);
    _postCustomerReply = _qnam.post(request, data);
    connect(_postCustomerReply, SIGNAL(finished()), this, SLOT(handlePostCustomerFinished()));
}

void PayWidget::handlePostCustomerFinished()
{
    qDebug() << "PayWidget::handlePostCustomerFinished()";
    _nextAction->setEnabled(true);
    _previousAction->setEnabled(true);
    if (_postCustomerReply != NULL) {
        _postCustomerReply->close();
        _postCustomerReply->deleteLater();
        if (_postCustomerReply->error() == QNetworkReply::NoError) {
            QByteArray json = _postCustomerReply->readAll();
            QJson::Parser parser;
            bool ok;
            QVariantMap customer = parser.parse (json, &ok).toMap();
            if (!ok) {
                qDebug() << "some error happend on server";
                return;
            }
            _customer = Customer::fromJson(customer);
            postConsume();
        } else if (_postCustomerReply->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
            QMessageBox::critical(this, trUtf8("认证失败"), trUtf8("请退出系统重新认证！"));
        }
    }
}

void PayWidget::postConsume()
{
    qDebug() << "PayWidget::postConsume";

    _nextAction->setEnabled(false);
    _previousAction->setEnabled(false);
    if (_consume != NULL) delete _consume;

    QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/customers/%2/consumes")).arg(_managerId).arg(_customer->getCustomerId())));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
    QVariantMap variant;
    variant.insert("amount", _dueEditor->getDue());
    variant.insert("netPaid", _netPaid);
    QMap<CompanyProduct, int> selectedProducts = _dueEditor->getSelectedCompanyProducts();
    if (selectedProducts.size() != 0) {
        QVariantList companyProducts;
        QMapIterator<CompanyProduct, int> i(selectedProducts);
        while (i.hasNext()) {
            i.next();
            QVariantMap companyProduct;
            companyProduct.insert("companyProductId", i.key().getCompanyProductId());
            companyProduct.insert("quantity", i.value());
            companyProducts.append(companyProduct);
        }
        variant.insert("companyProducts", companyProducts);
    }

    bool ok;
    QJson::Serializer serializer;
    QByteArray data = serializer.serialize(variant, &ok);
    Q_ASSERT(ok);
    _postConsumeReply = _qnam.post(request, data);
    connect(_postConsumeReply, SIGNAL(finished()), this, SLOT(handlePostConsumeFinished()));
}

void PayWidget::handlePostConsumeFinished()
{
    qDebug() << "PayWidget::handlePostConsumeFinished()";
    _nextAction->setEnabled(true);
    _previousAction->setEnabled(true);
    if (_postConsumeReply != NULL) {
        _postConsumeReply->close();
        _postConsumeReply->deleteLater();
        if (_postConsumeReply->error() == QNetworkReply::NoError) {
            QByteArray json = _postConsumeReply->readAll();
            QJson::Parser parser;
            bool ok;
            QVariantMap consume = parser.parse (json, &ok).toMap();
            if (!ok) {
                qDebug() << "some error happend on server";
                return;
            }
            _consume = Consume::fromJson(consume);
            putTickets();
        } else if (_postConsumeReply->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
            QMessageBox::critical(this, trUtf8("认证失败"), trUtf8("请退出系统重新认证！"));
        }
    }
}

void PayWidget::putTickets()
{
    qDebug() << "PayWidget::putTickets()";
    _nextAction->setEnabled(false);
    _previousAction->setEnabled(false);
    UseRule *useRule = _netEditor->getSelectedUseRule();

    bool allPutted = true;
    for (QList<Ticket *>::iterator i = _tickets.begin(); i != _tickets.end(); ++i) {
        if ((*i)->getCheckState() == Qt::Checked) {
            if (!(*i)->getValidAfterUse() || (*i)->getSaveAfterUse() != 0) {
                putTicket(*i, useRule);
                allPutted = false;
                break;
            }
        }
    }

    if (allPutted) {
        _nextAction->setEnabled(true);
        _previousAction->setEnabled(true);
        stepFour();
    }
}

void PayWidget::putTicket(Ticket *ticket, UseRule *useRule)
{
    qDebug() << "PayWidget::putTicket()";
    QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/tickets/%2")).arg(_managerId).arg(ticket->getTicketId())));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
    QVariantMap variant;
    variant.insert("consumeId", _consume->getConsumeId());
    variant.insert("save", ticket->getSaveAfterUse());
    if (useRule != NULL) {
        variant.insert("ruleId", useRule->getRuleId());
    }
    if (ticket->getValidAfterUse()) {
        variant.insert("state", 0);
    } else {
        variant.insert("state", 1);
    }
    if (ticket->getType() == Ticket::Deduction) {
        variant.insert("leftDeduction", ticket->getLeftDeductionAfterUse());
    }
    
    bool ok;
    QJson::Serializer serializer;
    QByteArray data = serializer.serialize(variant, &ok);
    Q_ASSERT(ok);
    _putTicketReply = _qnam.put(request, data);
    connect(_putTicketReply, SIGNAL(finished()), this, SLOT(handlePutTicketFinished()));
}

void PayWidget::handlePutTicketFinished()
{
    qDebug() << "PayWidget::handlePutTicketFinished()";
    if (_putTicketReply != NULL) {
        _putTicketReply->close();
        _putTicketReply->deleteLater();
        if (_putTicketReply->error() == QNetworkReply::NoError) {
            QByteArray json = _putTicketReply->readAll();
            QJson::Parser parser;
            bool ok;
            QVariantMap ticket = parser.parse (json, &ok).toMap();
            if (!ok) {
                qDebug() << "some error happend on server";
                return;
            }
            Ticket *newTicket = Ticket::fromJson(ticket);
            qDebug() << "ticket putted: " << newTicket->getTicketId();

            // remove from _tickets
            QMutableListIterator<Ticket *> i(_tickets);
            while (i.hasNext()) {
                Ticket *t = i.next();
                if (t->getTicketId() == newTicket->getTicketId()) {
                    i.remove();
                    break;
                }
            }
            putTickets();
        } else if (_putTicketReply->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
            QMessageBox::critical(this, trUtf8("认证失败"), trUtf8("请退出系统重新认证！"));
        }
    }
}


void PayWidget::stepFour()
{
    initActionsForLastStep();
    connect(_issueAction, SIGNAL(triggered()), this, SLOT(postTicket()));
    connect(_cancelAction, SIGNAL(triggered()), this, SLOT(cancel()));

    _dueEditor->hide();
    _reduceEditor->hide();
    delete _netEditor;
    _netEditor = NULL;
    _issueEditor = new IssueEditor(_managerId, _managerName, _level, _token, _companyId, _companyName, _latitude, _longitude, _issueRules, _netPaid, this);
    _issueEditor->setGeometry(250, 0, 1024 - 250, 540 - 54);
    _issueEditor->show();

    QString currentStep = trUtf8("发优惠券");
    _stepIndicator->setCurrentStep(currentStep);

    int candidateRulesCount = _issueEditor->getCandidateRulesCount();
    QString info = trUtf8("实付: %1元, 可选发券规则%2条");
    info = info.arg((_netPaid) / 100.0, 0, 'f', 2).arg(candidateRulesCount);
    _payInfo->setText(info);
}

void PayWidget::handleSummaryUpdated(QString &info)
{
    _payInfo->setText(info);
}

