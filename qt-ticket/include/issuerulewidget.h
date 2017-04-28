#ifndef ISSUE_RULE_WIDGET_H
#define ISSUE_RULE_WIDGET_H
#include <QWidget>
#include <QToolBar>
#include <QModelIndex>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class TipsWidget;
class QListView;
class IssueRule;
class IssueRuleListModel;
class IssueRuleEditor;
class IssueRuleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IssueRuleWidget(qulonglong &managerId, QString &managerName, int &level, QString &token, qulonglong &companyId, QString &companyName, double &latitude, double &longitude, QWidget *parent = 0);
    ~IssueRuleWidget();

private slots:
    void onAdd();
    void onCut();
    void onEdit();
    void onSave();
    void onRuleSelected(const QModelIndex &);
    void handleGetRulesFinished();
    void handleRuleDeleted(qulonglong);
    void handleRulePosted(IssueRule *rule);
    void handleRulePutted(qulonglong, IssueRule *newRule);

private:
    void requestRules();

    TipsWidget *_tipsWidget;
    QListView *_rulesWidget;
    QToolBar   *_toolbar;
    QAction    *_saveAction;
    QAction    *_editAction;
    QAction    *_addAction;
    QAction    *_cutAction;
    IssueRuleEditor    *_editArea;

    IssueRuleListModel *_model;
    QList<IssueRule *> _rules;
    
    const qulonglong &_managerId;
    const QString &_managerName;
    const int &_level;
    const QString &_token;
    const qulonglong &_companyId;
    const QString &_companyName;
    const double &_latitude;
    const double &_longitude;

    QNetworkAccessManager _qnam;
    QNetworkReply *_getRulesReply;
};

#endif

