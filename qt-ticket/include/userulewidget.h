#ifndef USE_RULE_WIDGET_H
#define USE_RULE_WIDGET_H
#include <QWidget>
#include <QToolBar>
#include <QListView>
#include <QModelIndex>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class TipsWidget;
class UseRule;
class UseRuleListModel;
class UseRuleEditor;
class UseRuleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UseRuleWidget(qulonglong &managerId, QString &managerName, int &level, QString &token, qulonglong &companyId, QString &companyName, double &latitude, double &longitude, QWidget *parent = 0);
    ~UseRuleWidget();

private slots:
    void onAdd();
    void onCut();
    void onEdit();
    void onSave();
    void onRuleSelected(const QModelIndex &);
    void handleHttpFinished();
    void handleHttpError(QNetworkReply::NetworkError e);
    void handleRuleDeleted(qulonglong);
    void handleRulePosted(UseRule *rule);
    void handleRulePutted(qulonglong, UseRule *newRule);

private:
    void requestRules();

    TipsWidget *_tipsWidget;
    QListView *_rulesWidget;
    QToolBar   *_toolbar;
    QAction    *_saveAction;
    QAction    *_editAction;
    QAction    *_addAction;
    QAction    *_cutAction;
    UseRuleEditor    *_editArea;

    UseRuleListModel *_model;
    QList<UseRule *> _rules;
    
    const qulonglong &_managerId;
    const QString &_managerName;
    const int &_level;
    const QString &_token;
    const qulonglong &_companyId;
    const QString &_companyName;
    const double &_latitude;
    const double &_longitude;

    QNetworkAccessManager _qnam;
    QNetworkReply *_reply;
};

#endif
