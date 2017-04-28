#include "userulewidget.h"
#include "tipswidget.h"
#include "userulelistmodel.h"
#include "useruleeditor.h"
#include "utils.h"
#include <QListView>
#include <QAction>
#include <QMessageBox>
#include <QJson/Serializer>
#include <QJson/Parser>
#include <qDebug>

UseRuleWidget::UseRuleWidget(qulonglong &managerId, QString &managerName, int &level, QString &token, qulonglong &companyId, QString &companyName, double &latitude, double &longitude, QWidget *parent) :
    _managerId(managerId),
    _managerName(managerName),
    _level(level),
    _token(token),
    _companyId(companyId),
    _companyName(companyName),
    _latitude(latitude),
    _longitude(longitude),
    QWidget(parent)
{
    QString title = trUtf8("使用规则制定:");
    QList<QString> tips;
    tips << trUtf8("可以新增、修改、删除优惠券的使用规则；");
    tips << trUtf8("当用户在结账时使用优惠券时（包括自己发放的优惠券和合作商户发放的优惠券），商家可以根据使用规则来具体计算用户可以享受的优惠额度；");
    tips << trUtf8("最少消费：规定用户的消费额度必须达到一定金额才能使用优惠券；");
    tips << trUtf8("最多抵扣：每次消费使用优惠券所享受的优惠不能超过该设定值；");
    tips << trUtf8("类型：该规则适用于用户使用的优惠券类型；");
    tips << trUtf8("新折扣率：用户使用折扣券时，商家确定的新的折扣率，比如对某一合作商家发放的7折折扣券，在本商家使用时可以享受9折优惠；");
    tips << trUtf8("抵扣率：用户使用代金券时，商家确定的新的抵扣金额和代金券面值的比例，如抵扣率为80%时，用户持有10元的代金券，则在本商家可以抵扣8元；");
    tips << trUtf8("适用商户：该规则适用于哪些合作商户发行的优惠券；");
    tips << trUtf8("关于使用规则更详细的信息，请查看帮助；");
    
    _tipsWidget = new TipsWidget(title, tips, this);
    _tipsWidget->setGeometry(0, 0, 250, 600);

    _rulesWidget = new QListView(this);
    _rulesWidget->setObjectName(tr("rulesList"));
    _rulesWidget->setGeometry(250, 0, 250, 600);
    _model = new UseRuleListModel(_rules);
    _rulesWidget->setModel(_model);

    connect(_rulesWidget, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onRuleSelected(const QModelIndex &)));

    _toolbar = new QToolBar(this);
    _toolbar->setLayoutDirection(Qt::RightToLeft);
    _toolbar->setGeometry(500, 0, 1024 - 500, 54);
    _toolbar->setIconSize(QSize(52, 52));
    _addAction = _toolbar->addAction(QIcon(":/ticket/add.png"), trUtf8("添加"));
    _cutAction = _toolbar->addAction(QIcon(":/ticket/cut.png"), trUtf8("删除"));
    _saveAction = _toolbar->addAction(QIcon(":/ticket/save.png"), trUtf8("保存"));
    _editAction = _toolbar->addAction(QIcon(":/ticket/edit.png"), trUtf8("编辑"));
    _addAction->setEnabled(true);
    _cutAction->setEnabled(false);
    _saveAction->setEnabled(true);
    _editAction->setEnabled(false);

    _editArea = new UseRuleEditor(_managerId, _managerName, _level, _token, _companyId, _companyName, _latitude, _longitude, NULL, this);
    _editArea->setGeometry(500, 54, 1024 - 500, 600 - 54);

    connect(_addAction, SIGNAL(triggered()), this, SLOT(onAdd()));
    connect(_cutAction, SIGNAL(triggered()), this, SLOT(onCut()));
    connect(_saveAction, SIGNAL(triggered()), this, SLOT(onSave()));
    connect(_editAction, SIGNAL(triggered()), this, SLOT(onEdit()));

    requestRules();
}

UseRuleWidget::~UseRuleWidget()
{
}

void UseRuleWidget::requestRules()
{
    QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/useRules")).arg(_managerId)));
    request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
    _reply = _qnam.get(request);
    connect(_reply, SIGNAL(finished()), this, SLOT(handleHttpFinished()));
    connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handleHttpError(QNetworkReply::NetworkError)));
}

void UseRuleWidget::handleHttpFinished() {
    if (_reply != NULL) {
        _reply->close();
        _reply->deleteLater();

        QByteArray json = _reply->readAll();
        QJson::Parser parser;
        bool ok;
        QVariantList rules = parser.parse (json, &ok).toList();
        if (!ok) {
            qDebug() << "some error happend on server";
            return;
        }
        for (QVariantList::iterator it = rules.begin(); it != rules.end(); ++it) {
            _rules.append(UseRule::fromJson(*it));
            _model->itemAppended();
        }
    }
}

void UseRuleWidget::handleHttpError(QNetworkReply::NetworkError e) {
    qDebug() << "UseRuleWidget::handleHttpError(): " << _reply->errorString() << ", " << e;
    _reply->close();
    _reply->deleteLater();
    if (QNetworkReply::AuthenticationRequiredError == e) {
        qDebug() << "authentication error";
        QMessageBox::critical(this, trUtf8("认证失败"), trUtf8("请退出系统重新认证！"));
    }
}

void UseRuleWidget::onRuleSelected(const QModelIndex &index)
{
    qDebug() << "onRuleSelected: " << index.row();
    UseRule *selectedRule = _rules.at(index.row());
    qDebug() << "selectedRule: " << selectedRule->getRuleId();

    // we use deleteLater here to avoid crash because of _reply->deleteLater() 
    _editArea->deleteLater();

    _editArea = new UseRuleEditor(_managerId, _managerName, _level, _token, _companyId, _companyName, _latitude, _longitude, selectedRule, this);
    _editArea->setGeometry(500, 54, 1024 - 500, 600 - 54);
    _editArea->show();

    _addAction->setEnabled(true);
    _cutAction->setEnabled(true);
    _saveAction->setEnabled(false);
    _editAction->setEnabled(true);
}


void UseRuleWidget::onAdd()
{
    _rulesWidget->clearSelection();

    _addAction->setEnabled(true);
    _cutAction->setEnabled(false);
    _saveAction->setEnabled(true);
    _editAction->setEnabled(false);

    _editArea->deleteLater();
    _editArea = new UseRuleEditor(_managerId, _managerName, _level, _token, _companyId, _companyName, _latitude, _longitude, NULL, this);
    _editArea->setGeometry(500, 54, 1024 - 500, 600 - 54);
    _editArea->show();
}

void UseRuleWidget::onCut()
{
    connect(_editArea, SIGNAL(ruleDeleted(const qulonglong)), this, SLOT(handleRuleDeleted(const qulonglong)));
    _editArea->delRule();
}

void UseRuleWidget::onEdit()
{
    _addAction->setEnabled(true);
    _cutAction->setEnabled(true);
    _saveAction->setEnabled(true);
    _editAction->setEnabled(false);
    _editArea->setMode(UseRuleEditor::Editable);
}

void UseRuleWidget::onSave()
{
    connect(_editArea, SIGNAL(rulePutted(qulonglong, UseRule *)), this, SLOT(handleRulePutted(qulonglong, UseRule *)));
    connect(_editArea, SIGNAL(rulePosted(UseRule *)), this, SLOT(handleRulePosted(UseRule *)));
    _editArea->saveRule();
}

void UseRuleWidget::handleRuleDeleted(qulonglong ruleId)
{
    qDebug() << "UseRuleWidget::handleRuleDeleted: " << ruleId;

    QMutableListIterator<UseRule *> i(_rules);
    while (i.hasNext()) {
        UseRule *rule = i.next();
        if (rule->getRuleId() == ruleId) {
            i.remove();
            break;
        }
    }
    _model->refresh();
    _rulesWidget->clearSelection();

    _addAction->setEnabled(true);
    _cutAction->setEnabled(false);
    _saveAction->setEnabled(true);
    _editAction->setEnabled(false);

    _editArea->deleteLater();
    _editArea = new UseRuleEditor(_managerId, _managerName, _level, _token, _companyId, _companyName, _latitude, _longitude, NULL, this);
    _editArea->setGeometry(500, 54, 1024 - 500, 600 - 54);
    _editArea->show();
}

void UseRuleWidget::handleRulePosted(UseRule *rule)
{
    qDebug() << "UseRuleWidget::handleRulePosted: " << rule->getRuleId();
    bool found = false;
    int idx;
    for (idx = 0; idx < _rules.size(); ++idx) {
        if (_rules[idx]->getRuleId() == rule->getRuleId()) {
            found = true;
            break;
        }
    }
    if (!found) {
        _rules.append(rule);
        _model->itemAppended();
    }

    _rulesWidget->clearSelection();

    _addAction->setEnabled(true);
    _cutAction->setEnabled(false);
    _saveAction->setEnabled(true);
    _editAction->setEnabled(false);

    _editArea->deleteLater();
    _editArea = new UseRuleEditor(_managerId, _managerName, _level, _token, _companyId, _companyName, _latitude, _longitude, NULL, this);
    _editArea->setGeometry(500, 54, 1024 - 500, 600 - 54);
    _editArea->show();
}

void UseRuleWidget::handleRulePutted(qulonglong oldRuleId, UseRule *newRule)
{
    qDebug() << "UseRuleWidget::handleRulePutted: " << oldRuleId << ", " << newRule->getRuleId();
    int idx;
    for (idx = 0; idx < _rules.size(); ++idx) {
        if (_rules[idx]->getRuleId() == oldRuleId) break;
    }
    UseRule *oldRule = _rules[idx];
    _rules.replace(idx, newRule);
    delete oldRule;
    _model->refresh();
    _rulesWidget->clearSelection();

    _addAction->setEnabled(true);
    _cutAction->setEnabled(false);
    _saveAction->setEnabled(true);
    _editAction->setEnabled(false);

    _editArea->deleteLater();
    _editArea = new UseRuleEditor(_managerId, _managerName, _level, _token, _companyId, _companyName, _latitude, _longitude, NULL, this);
    _editArea->setGeometry(500, 54, 1024 - 500, 600 - 54);
    _editArea->show();
}

