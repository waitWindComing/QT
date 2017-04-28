#ifndef ASSOCIATE_WIDGET_H
#define ASSOCIATE_WIDGET_H
#include "company.h"
#include <QWidget>
#include <QToolBar>
#include <QLineEdit>
#include <QListView>
#include <QWebView>
#include <QProgressDialog>
#include <QModelIndex>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class TipsWidget;
class CompanyModel;
class AssociateWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AssociateWidget(qulonglong &managerId, QString &managerName, int &level, QString &token, qulonglong &companyId, QString &companyName, double &latitude, double &longitude, QWidget *parent = 0);
    ~AssociateWidget();

private slots:
    void onAdd();
    void handleRequestAddFinished();
    void onCut();
    void handleRequestCutFinished();
    void onCompanySelected(const QModelIndex &);
    void onAssociationSelected(const QModelIndex &);
    void handleRequestCompaniesFinished();
    void handleRequestAssociationsFinished();

private:
    void requestCompanies();
    void requestAssociations();

    TipsWidget *_tipsWidget;
    QLineEdit *_companyFilter;
    QListView *_companiesWidget;
    QListView *_associationsWidget;
    QWebView *_locationWidget;
    QToolBar   *_toolbar;
    QAction    *_addAction;
    QAction    *_cutAction;
    QProgressDialog *_waitingDialog;

    CompanyModel *_companiesModel;
    QList<Company> _companies;
    CompanyModel *_associationsModel;
    QList<Company> _associations;

    const qulonglong &_managerId;
    const QString &_managerName;
    const int &_level;
    const QString &_token;
    const qulonglong &_companyId;
    const QString &_companyName;
    const double &_latitude;
    const double &_longitude;

    QNetworkAccessManager _qnam;
    QNetworkReply *_reply1;
    QNetworkReply *_reply2;
};

#endif

