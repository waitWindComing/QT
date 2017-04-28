#include "querywidget.h"
#include "tipswidget.h"
#include "consumewidget.h"
#include "utils.h"
#include <QMessageBox>
#include <QJson/Serializer>
#include <QJson/Parser>
#include <qDebug>

QueryWidget::QueryWidget(qulonglong &managerId, QString &managerName, int &level, QString &token, qulonglong &companyId, QString &companyName, double &latitude, double &longitude, QWidget *parent) :
    QWidget(parent),
    _managerId(managerId),
    _managerName(managerName),
    _level(level),
    _token(token),
    _companyId(companyId),
    _companyName(companyName),
    _latitude(latitude),
    _longitude(longitude),
    _index(-1),
    _consumeWidget(NULL)
{
    QString title = trUtf8("查询流水:");
    QList<QString> tips;
    tips << trUtf8("查询用户的流水，从最近开始逐条显示；");
    tips << trUtf8("通过点击上一条和下一条按钮显示流水情况，包括用户消费、券的使用和券的发放信息；");
    
    _tipsWidget = new TipsWidget(title, tips, this);
    _tipsWidget->setGeometry(0, 0, 250, 600);

    _buttonBar = new QToolBar(this);
    _buttonBar->setLayoutDirection(Qt::RightToLeft);
    _buttonBar->setGeometry(250, 600 - 54, 1024 - 250, 54);
    _buttonBar->setIconSize(QSize(52, 52));

    _nextAction = _buttonBar->addAction(QIcon(":/ticket/next.png"), trUtf8("下一条"));
    _previousAction = _buttonBar->addAction(QIcon(":/ticket/previous.png"), trUtf8("上一条"));
    connect(_nextAction, SIGNAL(triggered()), this, SLOT(next()));
    connect(_previousAction, SIGNAL(triggered()), this, SLOT(previous()));

    requestConsume(0);
}

QueryWidget::~QueryWidget()
{
    for (QList<Consume *>::iterator i = _consumes.begin(); i!= _consumes.end(); ++i) {
        delete *i;
    }
}

void QueryWidget::requestConsume(qulonglong consumeId)
{
    qDebug() << "QueryWidget::requestConsume()";
    if (consumeId == 0) {
        QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/consumes/latest")).arg(_managerId)));
        request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
        _getConsumeReply = _qnam.get(request);
        connect(_getConsumeReply, SIGNAL(finished()), this, SLOT(handleGetConsumeFinished()));
    } else {
        QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/consumes/%2/previous")).arg(_managerId).arg(consumeId)));
        request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
        _getConsumeReply = _qnam.get(request);
        connect(_getConsumeReply, SIGNAL(finished()), this, SLOT(handleGetConsumeFinished()));
    }
    _nextAction->setEnabled(false);
    _previousAction->setEnabled(false);
    _waitingDialog = new QProgressDialog(trUtf8("请稍后..."), QString(), 0, 0, this);
    _waitingDialog->exec();
}

void QueryWidget::handleGetConsumeFinished()
{
    qDebug() << "QueryWidget::handleGetConsumeFinished()";
    _waitingDialog->cancel();
    if (_getConsumeReply != NULL) {
        _getConsumeReply->close();
        _getConsumeReply->deleteLater();
        if (_getConsumeReply->error() == QNetworkReply::NoError) {
            QByteArray json = _getConsumeReply->readAll();
            QJson::Parser parser;
            bool ok;
            QVariantMap var = parser.parse (json, &ok).toMap();
            if (!ok) {
                qDebug() << "some error happend on server";
                return;
            }
            Consume *consume = Consume::fromJson(var);
            _consumes.append(consume);
            _index++;
            if (_consumeWidget != NULL) delete _consumeWidget;
            _consumeWidget = new ConsumeWidget(consume, this);
            _consumeWidget->setGeometry(250, 0, 1024 - 250, 600 - 54);
            _consumeWidget->show();
            _nextAction->setEnabled(true);
            if (_index > 0) {
                _previousAction->setEnabled(true);
            }
        } else if (_getConsumeReply->error() == QNetworkReply::ContentNotFoundError) {
            QMessageBox::information(this, trUtf8("没有记录"), trUtf8("没有查询到记录！"));
            if (_index > 0) {
                _previousAction->setEnabled(true);
            }
        } else if (_getConsumeReply->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
            QMessageBox::critical(this, trUtf8("认证失败"), trUtf8("请退出系统重新认证！"));
        }
    }
}

void QueryWidget::next()
{
    if (_index == _consumes.size() - 1) {   // the last one
        Consume *consume = _consumes.last();
        requestConsume(consume->getConsumeId());
    } else {
        _index++;
        _previousAction->setEnabled(true);
        Consume *consume = _consumes.at(_index);
        if (_consumeWidget != NULL) delete _consumeWidget;
        _consumeWidget = new ConsumeWidget(consume, this);
        _consumeWidget->setGeometry(250, 0, 1024 - 250, 600 - 54);
        _consumeWidget->show();
    }
}

void QueryWidget::previous()
{
    _index--;
    _nextAction->setEnabled(true);
    if (_index == 0) {
        _previousAction->setEnabled(false);
    }
    Consume *consume = _consumes.at(_index);
    if (_consumeWidget != NULL) delete _consumeWidget;
    _consumeWidget = new ConsumeWidget(consume, this);
    _consumeWidget->setGeometry(250, 0, 1024 - 250, 600 - 54);
    _consumeWidget->show();
}

