#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "paywidget.h"
#include "prepaywidget.h"
#include "querywidget.h"
#include "utils.h"
#include <QMessageBox>
#include <QJson/Serializer>
#include <QJson/Parser>
#include <QByteArray>
#include <qDebug>

const QString MainWindow::activeStyle = "padding: 0;"
            "margin: 0; font-size: 14px;"
            "font-weight: normal;"
            "line-height: 1.42857143;"
            "text-align: center;"
            "white-space: nowrap;"
            "vertical-align: middle;"
            "border: none;"
            "border-top-left-radius: 4px;"
            "border-top-right-radius: 4px;"
            "border-bottom-left-radius: 0;"
            "border-bottom-right-radius: 0;"
            "color: #fff;"
            "background: qlineargradient(x1:0, y1:0,x2:0, y2:1, stop:0 #00a5e0, stop:1 #0080ae);";

const QString MainWindow::inactiveStyle = "padding: 0;"
            "margin: 0; font-size: 14px;"
            "font-weight: normal;"
            "line-height: 1.42857143;"
            "text-align: center;"
            "white-space: nowrap;"
            "vertical-align: middle;"
            "border: none;"
            "border-top-left-radius: 4px;"
            "border-top-right-radius: 4px;"
            "border-bottom-left-radius: 0;"
            "border-bottom-right-radius: 0;"
            "color: #fff;"
            "background: qlineargradient(x1:0, y1:0,x2:0, y2:1, stop:0 #4a4a4a, stop:1 #393939);";

MainWindow::MainWindow(qulonglong managerId, QString &managerName, int level, QString &token, qulonglong companyId, QString &companyName, double latitude, double longitude, QWidget *parent) :
    _managerId(managerId),
    _managerName(managerName),
    _level(level),
    _token(token),
    _companyId(companyId),
    _companyName(companyName),
    _latitude(latitude),
    _longitude(longitude),
    _payWidget(NULL),
    _prepayWidget(NULL),
    _queryWidget(NULL),
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowTitle(companyName);

    _payButton = new QPushButton(this);
    _payButton->setGeometry(100, 48, 114, 42);
    _payButton->setText(trUtf8("结账"));
    _payButton->setStyleSheet(activeStyle);
    connect(_payButton, SIGNAL(clicked()), this, SLOT(on_payButton_clicked()));

    _prepayButton = new QPushButton(this);
    _prepayButton->setGeometry(220, 48, 114, 42);
    _prepayButton->setText(trUtf8("预存结账"));
    _prepayButton->setStyleSheet(inactiveStyle);
    connect(_prepayButton, SIGNAL(clicked()), this, SLOT(on_prepayButton_clicked()));

    _queryButton = new QPushButton(this);
    _queryButton->setGeometry(340, 48, 114, 42);
    _queryButton->setText(trUtf8("查询"));
    _queryButton->setStyleSheet(inactiveStyle);
    connect(_queryButton, SIGNAL(clicked()), this, SLOT(on_queryButton_clicked()));

    _payWidget = new PayWidget(_managerId, _managerName, _level, _token, _companyId, _companyName, _latitude, _longitude, this);
    _payWidget->setGeometry(0, 100, 1024, 600);
    _payWidget->show();

    requestBalance();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_payButton_clicked()
{
    _payButton->setStyleSheet(activeStyle);
    _prepayButton->setStyleSheet(inactiveStyle);
    _queryButton->setStyleSheet(inactiveStyle);

    removeWidgets();
    _payWidget = new PayWidget(_managerId, _managerName, _level, _token, _companyId, _companyName, _latitude, _longitude, this);
    _payWidget->setGeometry(0, 100, 1024, 600);
    _payWidget->show();
}

void MainWindow::on_prepayButton_clicked()
{
    _payButton->setStyleSheet(inactiveStyle);
    _prepayButton->setStyleSheet(activeStyle);
    _queryButton->setStyleSheet(inactiveStyle);

    removeWidgets();
    _prepayWidget = new PrepayWidget(_managerId, _managerName, _level, _token, _companyId, _companyName, _latitude, _longitude, this);
    _prepayWidget->setGeometry(0, 100, 1024, 600);
    _prepayWidget->show();
}

void MainWindow::on_queryButton_clicked()
{
    _payButton->setStyleSheet(inactiveStyle);
    _prepayButton->setStyleSheet(inactiveStyle);
    _queryButton->setStyleSheet(activeStyle);
    removeWidgets();
    _queryWidget = new QueryWidget(_managerId, _managerName, _level, _token, _companyId, _companyName, _latitude, _longitude, this);
    _queryWidget->setGeometry(0, 100, 1024, 600);
    _queryWidget->show();
}

void MainWindow::on_pushButton_clicked()
{
    QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/authentication")).arg(_managerId)));
    request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
    _logoutReply = _qnam.deleteResource(request);
    connect(_logoutReply, SIGNAL(finished()), this, SLOT(handleHttpLogoutFinished()));
}

void MainWindow::handleHttpLogoutFinished() {
    if (_logoutReply != NULL) {
        _logoutReply->close();
        _logoutReply->deleteLater();
        if (_logoutReply->error() == QNetworkReply::NoError) {
        } else if (_logoutReply->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
        }
    }
    exit(0);
}

void MainWindow::removeWidgets()
{
    if (_payWidget) {
        _payWidget->deleteLater();
        _payWidget = NULL;
    }
    if (_prepayWidget) {
        _prepayWidget->deleteLater();
        _prepayWidget = NULL;
    }
    if (_queryWidget) {
        _queryWidget->deleteLater();
        _queryWidget = NULL;
    }
}

void MainWindow::requestBalance()
{
    QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/balance")).arg(_managerId)));
    request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
    _balanceReply = _qnam.get(request);
    connect(_balanceReply, SIGNAL(finished()), this, SLOT(handleHttpBalanceFinished()));
}

void MainWindow::handleHttpBalanceFinished() {
    if (_balanceReply != NULL) {
        _balanceReply->close();
        _balanceReply->deleteLater();
        if (_balanceReply->error() == QNetworkReply::NoError) {
            QByteArray json = _balanceReply->readAll();
            QJson::Parser parser;
            bool ok;
            QVariantMap var = parser.parse (json, &ok).toMap();
            if (!ok) {
                qDebug() << "some error happend on server";
                return;
            }
            int balance = var["balance"].toInt();
            if (balance < 1000) {
                QMessageBox::critical(this, trUtf8("充值提醒"), trUtf8("您的账户余额已经低于10元，为了不影响您的正常使用，请尽快充值！"));
            } else if (balance < 5000) {
                QMessageBox::warning(this, trUtf8("充值提醒"), trUtf8("您的账户余额已经低于50元，为了不影响您的正常使用，请尽快充值！"));
            }
        } else if (_balanceReply->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
        }
    }
}

