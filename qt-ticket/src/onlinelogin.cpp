#include "onlinelogin.h"
#include "mainwindow.h"
#include "digitinputpanel.h"
#include "digitinputcontext.h"
#include "company.h"
#include "utils.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLayoutItem>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QMessageBox>
#include <QPixmap>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QRegExpValidator>
#include <QDesktopServices>
#include <QJson/Serializer>
#include <QJson/Parser>
#include <qDebug>

OnlineLogin::OnlineLogin(QWidget *parent) :
    QDialog(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);    

    QVBoxLayout *left = new QVBoxLayout;
    left->setContentsMargins(0, 0, 0, 0);

    QLabel *iconLabel = new QLabel(this);
    iconLabel->setPixmap(QPixmap(":/ticket/th.png").scaled(50, 50));
    iconLabel->setFixedSize(QSize(50, 50));
    left->addWidget(iconLabel);
    left->addSpacing(20);

    QHBoxLayout *companyIdLayout = new QHBoxLayout;
    QLabel *companyIdLabel = new QLabel(trUtf8("商户编码:"), this);
    companyIdLabel->setFixedSize(QSize(70, 30));
    companyIdLayout->addWidget(companyIdLabel);
    _companyIdEdit = new QLineEdit(this);
    _companyIdEdit->setFixedSize(QSize(240, 50));
    _companyIdEdit->setFont(QFont("Timers", 24));
    _companyIdEdit->setValidator(new QRegExpValidator(QRegExp("\\d+"), this));
    companyIdLayout->addWidget(_companyIdEdit);
    left->addLayout(companyIdLayout);

    _companyNameLayout = new QHBoxLayout;
    QLabel *companyNameLabel = new QLabel(trUtf8("商户名称:"), this);
    companyNameLabel->setFixedSize(QSize(70, 30));
    _companyNameLayout->addWidget(companyNameLabel);
    QLabel *holder = new QLabel(this);
    holder->setFixedSize(QSize(240, 30));
    _companyNameLayout->addWidget(holder);
    left->addLayout(_companyNameLayout);

    QHBoxLayout *managerIdLayout = new QHBoxLayout;
    QLabel *managerIdLabel = new QLabel(trUtf8("操作员编码:"), this);
    managerIdLabel->setFixedSize(QSize(70, 30));
    managerIdLayout->addWidget(managerIdLabel);
    _managerIdEdit = new QLineEdit(this);
    _managerIdEdit->setValidator(new QRegExpValidator(QRegExp("\\d+"), this));
    _managerIdEdit->setFixedSize(QSize(240, 50));
    _managerIdEdit->setFont(QFont("Timers", 24));
    managerIdLayout->addWidget(_managerIdEdit);
    left->addLayout(managerIdLayout);

    _managerNameLayout = new QHBoxLayout;
    QLabel *managerNameLabel = new QLabel(trUtf8("操作员名称:"), this);
    managerNameLabel->setFixedSize(QSize(70, 30));
    _managerNameLayout->addWidget(managerNameLabel);
    holder = new QLabel(this);
    holder->setFixedSize(QSize(240, 30));
    _managerNameLayout->addWidget(holder);
    left->addLayout(_managerNameLayout);

    QHBoxLayout *passwordLayout = new QHBoxLayout;
    QLabel *passwordLabel = new QLabel(trUtf8("登录密码:"), this);
    passwordLabel->setFixedSize(QSize(70, 30));
    passwordLayout->addWidget(passwordLabel);
    _passwordEdit = new QLineEdit(this);
    _passwordEdit->setEchoMode(QLineEdit::Password);
    _passwordEdit->setFixedSize(QSize(240, 50));
    _passwordEdit->setFont(QFont("Timers", 24));
    passwordLayout->addWidget(_passwordEdit);
    left->addLayout(passwordLayout);

    left->addSpacing(15);
    QPushButton *okButton = new QPushButton(trUtf8("确认"));
    QPushButton *cancelButton = new QPushButton(trUtf8("取消"));
    okButton->setFixedWidth(90);
    cancelButton->setFixedWidth(90);
    okButton->setFixedHeight(60);
    cancelButton->setFixedHeight(60);
    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    left->addLayout(buttonsLayout);

    QVBoxLayout *right = new QVBoxLayout;
    right->addSpacing(100);
    DigitInputPanel *inputPanel = new DigitInputPanel(this);
    inputPanel->setFixedSize(220, 270);
    right->addWidget(inputPanel);

    DigitInputContext *ic = new DigitInputContext(inputPanel, this);
    _companyIdEdit->setInputContext(ic);
    _managerIdEdit->setInputContext(ic);
    _passwordEdit->setInputContext(ic);

    QLabel *web = new QLabel(this);
    web->setText("<a href = http://www.elable.net>WWW.ELABLE.NET</a>");
    web->setOpenExternalLinks(true);
    web->setAlignment(Qt::AlignRight);
    right->addSpacing(20);
    right->addWidget(web);

    connect(_companyIdEdit, SIGNAL(editingFinished()), this, SLOT(getCompany()));
    connect(_managerIdEdit, SIGNAL(editingFinished()), this, SLOT(getManager()));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->addLayout(left);
    mainLayout->addLayout(right);
    setLayout(mainLayout);
}

OnlineLogin::~OnlineLogin()
{
}

void OnlineLogin::showEvent(QShowEvent *event)
{
    QSqlQuery query(QSqlDatabase::database());
    query.exec("SELECT companyId, managerId FROM User");
    if (query.next()) {
        _companyIdEdit->setText(query.value(0).toString());
        _managerIdEdit->setText(query.value(1).toString());
        getCompany();
        getManager();
        _passwordEdit->setFocus();
    } else {
        _companyIdEdit->setFocus();
    }
}

void OnlineLogin::accept()
{
    QNetworkRequest request(QUrl(Utils::getHost().append("/managers/authentication")));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QVariantMap variant;
    variant.insert("companyId", _companyIdEdit->text());
    variant.insert("managerId", _managerIdEdit->text());
    variant.insert("password", _passwordEdit->text());

    QJson::Serializer serializer;
    bool ok;
    QByteArray data = serializer.serialize(variant, &ok);
    Q_ASSERT(ok);
    _loginReply = _qnam.post(request, data);
    connect(_loginReply, SIGNAL(finished()), this, SLOT(handleLoginFinished()));
    _waitingDialog = new QProgressDialog(trUtf8("请稍后..."), QString(), 0, 0, this);
    _waitingDialog->exec();
}

void OnlineLogin::handleLoginFinished() {
    qDebug() << "OnlineLogin::handleLoginFinished()";
    if (_waitingDialog != NULL) {
        _waitingDialog->cancel();
        _waitingDialog = NULL;
    }
    if (_loginReply != NULL) {
        _loginReply->close();
        _loginReply->deleteLater();
        if (_loginReply->error() == QNetworkReply::NoError) {
            QByteArray json = _loginReply->readAll();
            QJson::Parser parser;
            bool ok;
            QVariantMap jmanager = parser.parse (json, &ok).toMap();
            if (!ok) {
                qDebug() << "some error happend on server";
                return;
            }
            qulonglong managerId = jmanager["id"].toULongLong();
            QString managerName = jmanager["name"].toString();
            int level = jmanager["level"].toInt();
            QString token = jmanager["token"].toString();
            qulonglong companyId = jmanager["company"].toMap()["id"].toULongLong();
            QString companyName = jmanager["company"].toMap()["name"].toString();
            double latitude = jmanager["company"].toMap()["latitude"].toDouble();
            double longitude = jmanager["company"].toMap()["longitude"].toDouble();

            // update local database
            QSqlQuery query(QSqlDatabase::database());
            query.exec("DELETE FROM User");
            query.exec(QString("INSERT INTO User VALUES ('%1', '%2')").arg(companyId).arg(managerId));

            close();
            MainWindow *mw = new MainWindow(managerId, managerName, level, token, companyId, companyName, latitude, longitude);
            mw->move ((QApplication::desktop()->width() - mw->width())/2,(QApplication::desktop()->height() - mw->height())/2);
            mw->show();
        } else if (_loginReply->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
            QMessageBox::information(this, trUtf8("认证错误"), trUtf8("用户名密码错误，请重新输入！"));
        }
    }
}

void OnlineLogin::getCompany()
{
    qDebug() << "OnlineLogin::getCompany()";
    _companyNameLayout->takeAt(1);
    QProgressBar *waitingCompanyName = new QProgressBar(this);
    waitingCompanyName->setFixedSize(QSize(240, 30));
    waitingCompanyName->setMinimum(0);
    waitingCompanyName->setMaximum(0);
    _companyNameLayout->addWidget(waitingCompanyName);
    _companyNameLayout->update();
    qulonglong companyId = _companyIdEdit->text().toULongLong();
    QNetworkRequest request(QUrl((Utils::getHost().append("/companies/%1")).arg(companyId)));
    _companyReply = _qnam.get(request);
    connect(_companyReply, SIGNAL(finished()), this, SLOT(handleGetCompanyFinished()));
}

void OnlineLogin::getManager()
{
    qDebug() << "OnlineLogin::getManager()";
    _managerNameLayout->takeAt(1);
    QProgressBar *waitingManagerName = new QProgressBar(this);
    waitingManagerName->setFixedSize(QSize(240, 30));
    waitingManagerName->setMinimum(0);
    waitingManagerName->setMaximum(0);
    _managerNameLayout->addWidget(waitingManagerName);
    _managerNameLayout->update();
    qulonglong companyId = _companyIdEdit->text().toULongLong();
    qulonglong managerId = _managerIdEdit->text().toULongLong();
    QNetworkRequest request(QUrl((Utils::getHost().append("/companies/%1/managers/%2")).arg(companyId).arg(managerId)));
    _managerReply = _qnam.get(request);
    connect(_managerReply, SIGNAL(finished()), this, SLOT(handleGetManagerFinished()));
}

void OnlineLogin::handleGetCompanyFinished()
{
    qDebug() << "OnlineLogin::handleGetCompanyFinished()";
    _companyNameLayout->takeAt(1);
    QLineEdit *companyNameEdit = new QLineEdit(this);
    companyNameEdit->setEnabled(false);
    companyNameEdit->setFixedSize(QSize(240, 30));
    _companyNameLayout->addWidget(companyNameEdit);
    _companyNameLayout->update();
    if (_companyReply != NULL) {
        _companyReply->close();
        _companyReply->deleteLater();
        if (_companyReply->error() == QNetworkReply::NoError) {
            QByteArray json = _companyReply->readAll();
            QJson::Parser parser;
            bool ok;
            QVariantMap jcompany = parser.parse (json, &ok).toMap();
            if (!ok) {
                qDebug() << "some error happend on server";
                return;
            }
            Company company = Company::fromJson(jcompany);
            companyNameEdit->setText(company.getName());
        } else {
            qDebug() << "get company error" << _companyReply->error();
        }
    }
}

void OnlineLogin::handleGetManagerFinished()
{
    qDebug() << "OnlineLogin::handleGetManagerFinished()";
    _managerNameLayout->takeAt(1);
    QLineEdit *managerNameEdit = new QLineEdit(this);
    managerNameEdit->setEnabled(false);
    managerNameEdit->setFixedSize(QSize(240, 30));
    _managerNameLayout->addWidget(managerNameEdit);
    _managerNameLayout->update();
    if (_managerReply != NULL) {
        _managerReply->close();
        _managerReply->deleteLater();
        if (_managerReply->error() == QNetworkReply::NoError) {
            QByteArray json = _managerReply->readAll();
            QJson::Parser parser;
            bool ok;
            QVariantMap jmanager = parser.parse (json, &ok).toMap();
            if (!ok) {
                qDebug() << "some error happend on server";
                return;
            }
            managerNameEdit->setText(jmanager["name"].toString());
        }
    }
}


void OnlineLogin::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.moveTo(0, 0);
    path.lineTo(0, 60);
    path.cubicTo(30,80, 60,50, 90,50);
    path.lineTo(width(), 50);
    path.lineTo(width(), 0);
    path.lineTo(0, 0);
    painter.setBrush(Qt::black);
    painter.drawPath(path);

    path.moveTo(0, 0);
    path.lineTo(0, 60);
    path.cubicTo(30,80, 60,50, 90,50);
    path.lineTo(width(), 50);
    painter.setPen(QPen(QColor(0, 128, 174), 3, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
    path.setFillRule(Qt::WindingFill);
    painter.drawPath(path);
}

