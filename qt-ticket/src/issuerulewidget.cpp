#include "issuerulewidget.h"
#include "company.h"
#include "tipswidget.h"
#include "issueruleeditor.h"
#include "issuerulelistmodel.h"
#include "issuedeductionrule.h"
#include "issuediscountrule.h"
#include "utils.h"
#include <QListView>
#include <QAction>
#include <QMessageBox>
#include <QJson/Serializer>
#include <QJson/Parser>
#include <qDebug>

IssueRuleWidget::IssueRuleWidget(qulonglong &managerId, QString &managerName, int &level, QString &token, qulonglong &companyId, QString &companyName, double &latitude, double &longitude, QWidget *parent) :
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
    QString title = trUtf8("发行规则制定:");
    QList<QString> tips;
    tips << trUtf8("可以新增、修改、删除优惠券的发行规则；");
    tips << trUtf8("当用户的消费达到一定额度的时候，可以根据发行规则给用户发优惠券；");
    tips << trUtf8("有效期：从优惠券发放日起开始计算，用户持该优惠券在有效期内再次消费时可以使用该优惠券从而享受优惠；");
    tips << trUtf8("优惠券分为折扣券和代金券；");
    tips << trUtf8("折扣券有一次性折扣券和反复使用折扣券两种；");
    tips << trUtf8("代金券：返还给用户的代金券的额度将根据用户的实付金额、代金券发放规则中的额度、返券额度和最小单位进行计算；");
    tips << trUtf8("适用商户：根据该规则发放的优惠券可以在哪些商户使用（肯定可以在当前商户使用，也就是发行优惠券的商户必须接收该商户发行的优惠券）；");
    tips << trUtf8("关于发行规则更详细的信息，请查看帮助；");
    
    _tipsWidget = new TipsWidget(title, tips, this);
    _tipsWidget->setGeometry(0, 0, 250, 600);

    _rulesWidget = new QListView(this);
    _rulesWidget->setObjectName(tr("rulesList"));
    _rulesWidget->setGeometry(250, 0, 250, 600);
    _model = new IssueRuleListModel(_rules);
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

    _editArea = new IssueRuleEditor(_managerId, _managerName, _level, _token, _companyId, _companyName, _latitude, _longitude, NULL, this);
    _editArea->setGeometry(500, 54, 1024 - 500, 600 - 54);

    connect(_addAction, SIGNAL(triggered()), this, SLOT(onAdd()));
    connect(_cutAction, SIGNAL(triggered()), this, SLOT(onCut()));
    connect(_saveAction, SIGNAL(triggered()), this, SLOT(onSave()));
    connect(_editAction, SIGNAL(triggered()), this, SLOT(onEdit()));

    requestRules();
}

IssueRuleWidget::~IssueRuleWidget()
{
    for (QList<IssueRule *>::iterator it = _rules.begin(); it != _rules.end(); ++it) {
        delete *it;
    }
}

void IssueRuleWidget::requestRules()
{
    QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/issueRules")).arg(_managerId)));
    request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
    _getRulesReply = _qnam.get(request);
    connect(_getRulesReply, SIGNAL(finished()), this, SLOT(handleGetRulesFinished()));
}


void IssueRuleWidget::handleGetRulesFinished() {
    if (_getRulesReply != NULL) {
        _getRulesReply->close();
        _getRulesReply->deleteLater();
        if (_getRulesReply->error() == QNetworkReply::NoError) {
            QByteArray json = _getRulesReply->readAll();
            QJson::Parser parser;
            bool ok;
            QVariantList rules = parser.parse (json, &ok).toList();
            if (!ok) {
                qDebug() << "some error happend on server";
                return;
            }
            for (QVariantList::iterator it = rules.begin(); it != rules.end(); ++it) {
                _rules.append(IssueRule::fromJson(*it));
                _model->itemAppended();
            }
        } else if (_getRulesReply->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
            QMessageBox::critical(this, trUtf8("认证失败"), trUtf8("请退出系统重新认证！"));
        }
    }
}

void IssueRuleWidget::onRuleSelected(const QModelIndex &index)
{
    qDebug() << "onRuleSelected: " << index.row();
    IssueRule *selectedRule = _rules.at(index.row());
    qDebug() << "selectedRule: " << selectedRule->getRuleId();

    // we use deleteLater here to avoid crash because of _reply->deleteLater() 
    _editArea->deleteLater();

    _editArea = new IssueRuleEditor(_managerId, _managerName, _level, _token, _companyId, _companyName, _latitude, _longitude, selectedRule, this);
    _editArea->setGeometry(500, 54, 1024 - 500, 600 - 54);
    _editArea->show();

    _addAction->setEnabled(true);
    _cutAction->setEnabled(true);
    _saveAction->setEnabled(false);
    _editAction->setEnabled(true);
}

void IssueRuleWidget::onAdd()
{
    _rulesWidget->clearSelection();

    _addAction->setEnabled(true);
    _cutAction->setEnabled(false);
    _saveAction->setEnabled(true);
    _editAction->setEnabled(false);

    _editArea->deleteLater();
    _editArea = new IssueRuleEditor(_managerId, _managerName, _level, _token, _companyId, _companyName, _latitude, _longitude, NULL, this);
    _editArea->setGeometry(500, 54, 1024 - 500, 600 - 54);
    _editArea->show();
}

void IssueRuleWidget::onCut()
{
    connect(_editArea, SIGNAL(ruleDeleted(const qulonglong)), this, SLOT(handleRuleDeleted(const qulonglong)));
    _editArea->delRule();
}

void IssueRuleWidget::onEdit()
{
    _addAction->setEnabled(true);
    _cutAction->setEnabled(true);
    _saveAction->setEnabled(true);
    _editAction->setEnabled(false);
    _editArea->setMode(IssueRuleEditor::Editable);
}

void IssueRuleWidget::onSave()
{
    connect(_editArea, SIGNAL(rulePutted(qulonglong, IssueRule *)), this, SLOT(handleRulePutted(qulonglong, IssueRule *)));
    connect(_editArea, SIGNAL(rulePosted(IssueRule *)), this, SLOT(handleRulePosted(IssueRule *)));
    _editArea->saveRule();
}

void IssueRuleWidget::handleRuleDeleted(qulonglong ruleId)
{
    qDebug() << "IssueRuleWidget::handleRuleDeleted: " << ruleId;

    QMutableListIterator<IssueRule *> i(_rules);
    while (i.hasNext()) {
        IssueRule *rule = i.next();
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
    _editArea = new IssueRuleEditor(_managerId, _managerName, _level, _token, _companyId, _companyName, _latitude, _longitude, NULL, this);
    _editArea->setGeometry(500, 54, 1024 - 500, 600 - 54);
    _editArea->show();
}

void IssueRuleWidget::handleRulePosted(IssueRule *rule)
{
    qDebug() << "IssueRuleWidget::handleRulePosted: " << rule->getRuleId();
    int idx;
    bool found = false;
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
    _editArea = new IssueRuleEditor(_managerId, _managerName, _level, _token, _companyId, _companyName, _latitude, _longitude, NULL, this);
    _editArea->setGeometry(500, 54, 1024 - 500, 600 - 54);
    _editArea->show();
}

void IssueRuleWidget::handleRulePutted(qulonglong oldRuleId, IssueRule *newRule)
{
    qDebug() << "IssueRuleWidget::handleRulePutted: " << oldRuleId << ", " << newRule->getRuleId();
    int idx;
    for (idx = 0; idx < _rules.size(); ++idx) {
        if (_rules[idx]->getRuleId() == oldRuleId) break;
    }
    IssueRule *oldRule = _rules[idx];
    _rules.replace(idx, newRule);
    delete oldRule;
    _model->refresh();
    _rulesWidget->clearSelection();

    _addAction->setEnabled(true);
    _cutAction->setEnabled(false);
    _saveAction->setEnabled(true);
    _editAction->setEnabled(false);

    _editArea->deleteLater();
    _editArea = new IssueRuleEditor(_managerId, _managerName, _level, _token, _companyId, _companyName, _latitude, _longitude, NULL, this);
    _editArea->setGeometry(500, 54, 1024 - 500, 600 - 54);
    _editArea->show();
}

