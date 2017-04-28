#include "prepaywidget.h"
#include "stepindicator.h"
#include "tipswidget.h"
#include "dueeditor.h"
#include "preneteditor.h"
#include "confirmcodewidget.h"
#include "prepayresultdlg.h"
#include "companycustomer.h"
#include "customer.h"
#include "consume.h"
#include "utils.h"
#include <QToolBar>
#include <QMessageBox>
#include <QMap>
#include <QJson/Serializer>
#include <QJson/Parser>
#include <qDebug>

PrepayWidget::PrepayWidget(qulonglong &managerId, QString &managerName, int &level, QString &token, qulonglong &companyId, QString &companyName, double &latitude, double &longitude, QWidget *parent) :
    QWidget(parent),
    _managerId(managerId),
    _managerName(managerName),
    _level(level),
    _token(token),
    _companyId(companyId),
    _companyName(companyName),
    _latitude(latitude),
    _longitude(longitude),
    _stepIndicator(NULL),
    _netEditor(NULL),
    _companyCustomer(NULL)
{
    QString title = trUtf8("消费者本次消费:");
    QList<QString> tips;
    tips << trUtf8("输入消费者的手机号码；");
    tips << trUtf8("本次的消费额度（应付）；");
    tips << trUtf8("该消费者必须是本商户的预存用户；");
    
    _tipsWidget = new TipsWidget(title, tips, this);
    _tipsWidget->setGeometry(0, 0, 250, 540);

    QList<QString> steps;
    steps << trUtf8("应付") << trUtf8("实付") << trUtf8("扣款");
    QString currentStep = trUtf8("应付");
    _stepIndicator = new StepIndicator(steps, currentStep, this);
    _stepIndicator->setGeometry(0, 540, 1024, 60);
    _stepIndicator->show();

    _buttonBar = new QToolBar(this);
    _buttonBar->setLayoutDirection(Qt::RightToLeft);
    _buttonBar->setGeometry(250, 540 - 54, 1024 - 250, 54);
    _buttonBar->setIconSize(QSize(52, 52));
    initActionsForFirstStep();

    _dueEditor = new DueEditor(_managerId, _managerName, _level, _token, _companyId, _companyName, _latitude, _longitude, this);
    _dueEditor->setGeometry(250, 0, 1024 - 250, 540 - 54);
    connect(_dueEditor, SIGNAL(editingFinished()), this, SLOT(handleDueEditingFinshed()));
    connect(_dueEditor, SIGNAL(editingUnFinished()), this, SLOT(handleDueEditingUnFinshed()));

    QLabel *memberLabel = new QLabel(this);
    memberLabel->setPixmap(QPixmap(":/ticket/member.png"));
    memberLabel->setGeometry(560, 30, 128, 104);

    goStepOne();
}

PrepayWidget::~PrepayWidget()
{
    delete _dueEditor;
    if (_netEditor != NULL) delete _netEditor;
    if (_stepIndicator != NULL) delete _stepIndicator;
}

void PrepayWidget::handleDueEditingFinshed()
{
    _nextAction->setEnabled(true);
    _nextAction->disconnect();
    connect(_nextAction, SIGNAL(triggered()), this, SLOT(requestCompanyCustomer()));
}

void PrepayWidget::handleDueEditingUnFinshed()
{
    _nextAction->setEnabled(false);
    _nextAction->disconnect();
}

void PrepayWidget::goStepOne()
{
    qDebug() << "goStepOne()";
    if (sender() == _previousAction) {
        _nextAction->setEnabled(true);
        _nextAction->disconnect();
        connect(_nextAction, SIGNAL(triggered()), this, SLOT(requestCompanyCustomer()));
    } else {
        if (_companyCustomer) {
            delete _companyCustomer;
            _companyCustomer = NULL;
        }
        _dueEditor->clear();
        _nextAction->setEnabled(false);
    }
    _previousAction->setEnabled(false);
    if (_netEditor != NULL) {
        delete _netEditor;
        _netEditor = NULL;
    }

    _dueEditor->show();
    QString currentStep = trUtf8("应付");
    _stepIndicator->setCurrentStep(currentStep);
}

void PrepayWidget::goStepTwo()
{
    qDebug() << "goStepTwo()";

    int balance = _companyCustomer->getBalance();
    int due = _dueEditor->getDue();
    int discount = _companyCustomer->getDiscount();
    float net = due * discount / 100.0;
    if (balance < net) {   // not enough balance
        _nextAction->setEnabled(false);
    } else {
        _nextAction->setEnabled(true);
        _nextAction->disconnect();
        connect(_nextAction, SIGNAL(triggered()), this, SLOT(goStepThree()));
    }

    _previousAction->setEnabled(true);
    _previousAction->disconnect();
    connect(_previousAction, SIGNAL(triggered()), this, SLOT(goStepOne()));

    _dueEditor->hide();
    _netEditor = new PreNetEditor(_managerId, _managerName, _level, _token, _companyId, _companyName, _latitude, _longitude, due, _companyCustomer, this);
    _netEditor->setGeometry(250, 0, 1024 - 250, 540 - 54);
    _netEditor->show();
    QString currentStep = trUtf8("实付");
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

void PrepayWidget::goStepThree()
{
    qDebug() << "goStepThree()";
    _nextAction->setEnabled(false);
    _previousAction->setEnabled(false);
    _confirmCodeWidget = new ConfirmCodeWidget(_managerId, _token, _companyCustomer->getCustomer()->getCustomerId(), this);
    _confirmCodeWidget->setGeometry(780, 150, 240, 400);
    connect(_confirmCodeWidget, SIGNAL(pass()), this, SLOT(handleConfirmCodePass()));
    connect(_confirmCodeWidget, SIGNAL(skip()), this, SLOT(handleConfirmCodeSkip()));
    _confirmCodeWidget->show();
    _confirmCodeWidget->focus();
    QString currentStep = trUtf8("扣款");
    _stepIndicator->setCurrentStep(currentStep);
}

void PrepayWidget::handleConfirmCodePass()
{
    qDebug() << "PrepayWidget::handleConfirmCodePass()";
    postConsume();
    _confirmCodeWidget->close();
    _confirmCodeWidget->deleteLater();
}

void PrepayWidget::handleConfirmCodeSkip()
{
    qDebug() << "PrepayWidget::handleConfirmCodeSkip()";
    _previousAction->setEnabled(true);
    _nextAction->setEnabled(true);
    _confirmCodeWidget->close();
    _confirmCodeWidget->deleteLater();
}

void PrepayWidget::requestCompanyCustomer()
{
    qDebug() << "PrepayWidget::requestCompanyCustomer()";
    _nextAction->setEnabled(false);
    _previousAction->setEnabled(false);
    if (_companyCustomer != NULL) {
        delete _companyCustomer;
        _companyCustomer = NULL;
    }
    QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/companyCustomers?customerName=%2")).arg(_managerId).arg(_dueEditor->getCustomer())));
    request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
    _getCompanyCustomerReply = _qnam.get(request);
    connect(_getCompanyCustomerReply, SIGNAL(finished()), this, SLOT(handleGetCompanyCustomerFinished()));
}

void PrepayWidget::handleGetCompanyCustomerFinished()
{
    qDebug() << "PrepayWidget::handleGetCompanyCustomerFinished()";
    _nextAction->setEnabled(true);
    _previousAction->setEnabled(true);
    if (_getCompanyCustomerReply != NULL) {
        _getCompanyCustomerReply->close();
        _getCompanyCustomerReply->deleteLater();
        if (_getCompanyCustomerReply->error() == QNetworkReply::NoError) {
            QByteArray json = _getCompanyCustomerReply->readAll();
            QJson::Parser parser;
            bool ok;
            QVariant companyCustomer = parser.parse (json, &ok);
            if (!ok) {
                qDebug() << "some error happend on server";
                return;
            }
            _companyCustomer = CompanyCustomer::fromJson(companyCustomer);
            Q_ASSERT(_companyCustomer != NULL);
            goStepTwo();
        } else if (_getCompanyCustomerReply->error() == QNetworkReply::ContentNotFoundError) {
            // pop dialog to warn and stay on this page
            QMessageBox::critical(this, trUtf8("无此用户"), trUtf8("该用户不是预存用户！"));
        } else if (_getCompanyCustomerReply->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
            QMessageBox::critical(this, trUtf8("认证失败"), trUtf8("请退出系统重新认证！"));
        }
    }
}

void PrepayWidget::initActionsForFirstStep()
{
    _buttonBar->setGeometry(250, 540 - 54, 1024 - 250, 54);
    _buttonBar->setIconSize(QSize(50, 50));
    _buttonBar->clear();
    _nextAction = _buttonBar->addAction(QIcon(":/ticket/next.png"), trUtf8("下一步"));
    _buttonBar->addSeparator();
    _previousAction = _buttonBar->addAction(QIcon(":/ticket/previous.png"), trUtf8("上一步"));
}

void PrepayWidget::initActionsForLastStep()
{
    _buttonBar->setGeometry(250, 540 - 54, 1024 - 250, 54);
    _buttonBar->setIconSize(QSize(50, 50));
    _buttonBar->clear();
    _previousAction = _buttonBar->addAction(QIcon(":/ticket/previous.png"), trUtf8("上一步"));
    _buttonBar->addSeparator();
    _endAction = _buttonBar->addAction(QIcon(":/ticket/end.png"), trUtf8("结束"));
}

void PrepayWidget::postConsume()
{
    qDebug() << "PrepayWidget::postConsume";

    _nextAction->setEnabled(false);
    _previousAction->setEnabled(false);

    QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/companyCustomers/%2/consumes")).arg(_managerId).arg(_companyCustomer->getCompanyCustomerId())));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());

    qlonglong balance = _companyCustomer->getBalance();
    qlonglong due = _dueEditor->getDue();
    int discount = _companyCustomer->getDiscount();
    qlonglong net = due * discount / 100;
    QVariantMap variant;
    variant.insert("amount", due);
    variant.insert("netDeduction", net);
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

void PrepayWidget::handlePostConsumeFinished()
{
    qDebug() << "PrepayWidget::handlePostConsumeFinished()";
    _nextAction->setEnabled(true);
    _previousAction->setEnabled(true);
    if (_postConsumeReply != NULL) {
        _postConsumeReply->close();
        _postConsumeReply->deleteLater();
        if (_postConsumeReply->error() == QNetworkReply::NoError) {
            QByteArray json = _postConsumeReply->readAll();
            QJson::Parser parser;
            bool ok;
            QVariantMap consumeVar = parser.parse (json, &ok).toMap();
            if (!ok) {
                qDebug() << "some error happend on server";
                return;
            }
            Consume *consume = Consume::fromJson(consumeVar);
            PrepayResultDlg dlg(consume, this);
            dlg.exec();
            delete consume;
            goStepOne();
        } else if (_postConsumeReply->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
            QMessageBox::critical(this, trUtf8("认证失败"), trUtf8("请退出系统重新认证！"));
        }
    }
}

