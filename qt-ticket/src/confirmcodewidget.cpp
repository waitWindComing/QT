#include "confirmcodewidget.h"
#include "digitinputpanel.h"
#include "digitinputcontext.h"
#include "utils.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QJson/Serializer>
#include <QJson/Parser>
#include <qDebug>

ConfirmCodeWidget::ConfirmCodeWidget(const qulonglong &managerId, const QString &token, qulonglong customerId, QWidget *parent)
    : _managerId(managerId),
    _customerId(customerId),
    _token(token),
    QWidget(parent)
{
    _codeEdit = new QLineEdit(this);
    _codeEdit->setFont(QFont("Timers", 24));
    _codeEdit->setFixedSize(220, 40);
    DigitInputPanel *inputPanel = new DigitInputPanel(this);
    inputPanel->setFixedSize(220, 270);
    DigitInputContext *ic = new DigitInputContext(inputPanel, this);
    _codeEdit->setInputContext(ic);

    _skipButton = new QPushButton(trUtf8("跳过"), this);
    _skipButton->setFixedHeight(50);
    _okButton = new QPushButton(trUtf8("确认"), this);
    _okButton->setFixedHeight(50);
    connect(_skipButton, SIGNAL(clicked()), this, SIGNAL(skip()));
    connect(_okButton, SIGNAL(clicked()), this, SLOT(check()));

    QVBoxLayout *lt = new QVBoxLayout;
    lt->addWidget(inputPanel);
    lt->addWidget(_codeEdit);
    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(_skipButton);
    buttonsLayout->addSpacing(15);
    buttonsLayout->addWidget(_okButton);
    lt->addLayout(buttonsLayout);
    setLayout(lt);
}

void ConfirmCodeWidget::focus()
{
    _codeEdit->setFocus();
}

void ConfirmCodeWidget::check()
{
    _skipButton->setEnabled(false);
    _okButton->setEnabled(false);

    QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/customers/%2/confirmcode")).arg(_managerId).arg(_customerId)));
    request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
    _getConfirmCodeReply = _qnam.get(request);
    connect(_getConfirmCodeReply, SIGNAL(finished()), this, SLOT(handleGetConfirmCodeFinished()));
    _waitingDialog = new QProgressDialog(trUtf8("请稍后..."), QString(), 0, 0, this);
    _waitingDialog->exec();
}

void ConfirmCodeWidget::handleGetConfirmCodeFinished()
{
    _waitingDialog->cancel();
    _skipButton->setEnabled(true);
    _okButton->setEnabled(true);

    if (_getConfirmCodeReply != NULL) {
        _getConfirmCodeReply->close();
        _getConfirmCodeReply->deleteLater();
        if (_getConfirmCodeReply->error() == QNetworkReply::NoError) {
            QByteArray json = _getConfirmCodeReply->readAll();
            QJson::Parser parser;
            bool ok;
            QVariantMap var = parser.parse (json, &ok).toMap();
            if (!ok) {
                qDebug() << "some error happend on server";
                return;
            }
            QString confirmCode = var["confirmCode"].toString();
            if (confirmCode == _codeEdit->text()) {
                emit pass();
            } else {
                _codeEdit->clear();
                QMessageBox::warning(this, trUtf8("确认码"), trUtf8("确认码错误！"));
            }
        } else if (_getConfirmCodeReply->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
            QMessageBox::critical(this, trUtf8("认证失败"), trUtf8("请退出系统重新认证！"));
        }
    }
}

