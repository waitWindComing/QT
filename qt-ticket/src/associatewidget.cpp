#include "associatewidget.h"
#include "tipswidget.h"
#include "companymodel.h"
#include "utils.h"
#include <QAction>
#include <QMessageBox>
#include <QJson/Serializer>
#include <QJson/Parser>
#include <qDebug>

AssociateWidget::AssociateWidget(qulonglong &managerId, QString &managerName, int &level, QString &token, qulonglong &companyId, QString &companyName, double &latitude, double &longitude, QWidget *parent) :
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
    QString title = trUtf8("合作商户管理:");
    QList<QString> tips;
    tips << trUtf8("添加、移除合作商户；");
    tips << trUtf8("若发行规则中指定了合作商户，则以此规则发行的优惠券将可能在这些合作商户中使用；");
    tips << trUtf8("若使用规则中指定了合作商户，则持有这些合作商户发行的优惠券的用户可以使用这些优惠券；");
    tips << trUtf8("关于合作商户更详细的信息，请查看帮助；");
    
    _tipsWidget = new TipsWidget(title, tips, this);
    _tipsWidget->setGeometry(0, 0, 250, 600);

    _companyFilter = new QLineEdit(this);
    _companyFilter->setObjectName(tr("filter"));
    _companyFilter->setGeometry(270, 60, 347, 30);

    _companiesWidget = new QListView(this);
    _companiesWidget->setGeometry(270, 100, 347, 490);
    _associationsWidget = new QListView(this);
    _associationsWidget->setGeometry(657, 60, 347, 260);
    connect(_companiesWidget, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onCompanySelected(const QModelIndex &)));
    connect(_associationsWidget, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onAssociationSelected(const QModelIndex &)));

    _companiesModel = new CompanyModel(_companies);
    _companiesWidget->setModel(_companiesModel);
    _associationsModel = new CompanyModel(_associations);
    _associationsWidget->setModel(_associationsModel);

    _locationWidget = new QWebView(this);
    _locationWidget->setGeometry(657, 330, 347, 250);
    _locationWidget->load(QUrl(Utils::generateLocation(_longitude, _latitude, _companyName)));

    _toolbar = new QToolBar(this);
    _toolbar->setLayoutDirection(Qt::RightToLeft);
    _toolbar->setGeometry(250, 0, 1024 - 250, 54);
    _toolbar->setIconSize(QSize(52, 52));
    _addAction = _toolbar->addAction(QIcon(":/ticket/add.png"), trUtf8("添加"));
    _cutAction = _toolbar->addAction(QIcon(":/ticket/cut.png"), trUtf8("删除"));
    _addAction->setEnabled(false);
    _cutAction->setEnabled(false);

    connect(_addAction, SIGNAL(triggered()), this, SLOT(onAdd()));
    connect(_cutAction, SIGNAL(triggered()), this, SLOT(onCut()));

    requestCompanies();
    requestAssociations();
}

AssociateWidget::~AssociateWidget()
{
}

void AssociateWidget::onAdd()
{
    QItemSelectionModel *selectionModel = _companiesWidget->selectionModel();
    QItemSelection itemSelection = selectionModel->selection();
    QModelIndexList selectedIndexes = itemSelection.indexes();
    for (QModelIndexList::iterator i = selectedIndexes.begin(); i != selectedIndexes.end(); ++i) {
        int idx = (*i).row();
        Company selectedCompany = _companies.at(idx);
        QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/associateCompanies")).arg(_managerId)));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
        QVariantMap variant;
        bool ok;
        variant.insert("id", selectedCompany.getCompanyId());
        variant.insert("name", selectedCompany.getName());
        variant.insert("latitude", selectedCompany.getLatitude());
        variant.insert("longitude", selectedCompany.getLongitude());
        QJson::Serializer serializer;
        QByteArray data = serializer.serialize(variant, &ok);
        Q_ASSERT(ok);
        _reply1 = _qnam.post(request, data);
        connect(_reply1, SIGNAL(finished()), this, SLOT(handleRequestAddFinished()));
        _waitingDialog = new QProgressDialog(trUtf8("请稍后..."), QString(), 0, 0, this);
        _waitingDialog->exec();
    }
}

void AssociateWidget::handleRequestAddFinished()
{
    _waitingDialog->cancel();
    if (_reply1 != NULL) {
        _reply1->close();
        _reply1->deleteLater();
        if (_reply1->error() == QNetworkReply::NoError) {
            QByteArray json = _reply1->readAll();
            QJson::Parser parser;
            bool ok;
            QVariant var = parser.parse (json, &ok);
            Company company = Company::fromJson(var);
            _associations.append(company);
            _associationsModel->itemAppended();
            _companies.removeAll(company);
            _companiesModel->refresh();
        } else if (_reply1->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
            QMessageBox::critical(this, trUtf8("认证失败"), trUtf8("请退出系统重新认证！"));
        } 
    }
}

void AssociateWidget::onCut()
{
    QItemSelectionModel *selectionModel = _associationsWidget->selectionModel();
    QItemSelection itemSelection = selectionModel->selection();
    QModelIndexList selectedIndexes = itemSelection.indexes();
    for (QModelIndexList::iterator i = selectedIndexes.begin(); i != selectedIndexes.end(); ++i) {
        int idx = (*i).row();
        Company selectedCompany = _associations.at((*i).row());
        QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/associateCompanies/%2")).arg(_managerId).arg(selectedCompany.getCompanyId())));
        request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
        _reply1 = _qnam.deleteResource(request);
        connect(_reply1, SIGNAL(finished()), this, SLOT(handleRequestCutFinished()));
        _waitingDialog = new QProgressDialog(trUtf8("请稍后..."), QString(), 0, 0, this);
        _waitingDialog->exec();
    }
}

void AssociateWidget::handleRequestCutFinished()
{
    _waitingDialog->cancel();
    if (_reply1 != NULL) {
        _reply1->close();
        _reply1->deleteLater();
        if (_reply1->error() == QNetworkReply::NoError) {
            QByteArray json = _reply1->readAll();
            QJson::Parser parser;
            bool ok;
            QVariant var = parser.parse (json, &ok);
            Company company = Company::fromJson(var);
            _companies.append(company);
            _companiesModel->itemAppended();
            _associations.removeAll(company);
            _associationsModel->refresh();
        } else if (_reply1->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
            QMessageBox::critical(this, trUtf8("认证失败"), trUtf8("请退出系统重新认证！"));
        } 
    }
}


void AssociateWidget::onCompanySelected(const QModelIndex &index)
{
    _associationsWidget->clearSelection();
    _addAction->setEnabled(true);
    _cutAction->setEnabled(false);
    Company company = _companies.at(index.row());
    _locationWidget->load(QUrl(Utils::generateLocation(company.getLongitude(), company.getLatitude(), company.getName())));
}

void AssociateWidget::onAssociationSelected(const QModelIndex &index)
{
    _companiesWidget->clearSelection();
    _addAction->setEnabled(false);
    _cutAction->setEnabled(true);
    Company company = _associations.at(index.row());
    _locationWidget->load(QUrl(Utils::generateLocation(company.getLongitude(), company.getLatitude(), company.getName())));
}

void AssociateWidget::requestCompanies()
{
    qDebug() << "AssociateWidget::requestCompanies";
    QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/companies")).arg(_managerId)));
    request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
    _reply1 = _qnam.get(request);
    connect(_reply1, SIGNAL(finished()), this, SLOT(handleRequestCompaniesFinished()));
    _waitingDialog = new QProgressDialog(trUtf8("请稍后..."), QString(), 0, 0, this);
    _waitingDialog->exec();
}

void AssociateWidget::requestAssociations()
{
    qDebug() << "AssociateWidget::requestCompanies";
    QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/associateCompanies")).arg(_managerId)));
    request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
    _reply2 = _qnam.get(request);
    connect(_reply2, SIGNAL(finished()), this, SLOT(handleRequestAssociationsFinished()));
    _waitingDialog = new QProgressDialog(trUtf8("请稍后..."), QString(), 0, 0, this);
    _waitingDialog->exec();
}

void AssociateWidget::handleRequestCompaniesFinished()
{
    _waitingDialog->cancel();
    if (_reply1 != NULL) {
        _reply1->close();
        _reply1->deleteLater();

        QByteArray json = _reply1->readAll();
        _reply1 = NULL;
        QJson::Parser parser;
        bool ok;
        QVariantList companies = parser.parse (json, &ok).toList();
        if (!ok) {
            qDebug() << "some error happend on server";
            return;
        }
        for (QVariantList::iterator i = companies.begin(); i != companies.end(); ++i) {
            Company company = Company::fromJson(*i);
            if (_associations.contains(company)) continue;
            if (company.getCompanyId() == _companyId) continue;
            _companies.append(company);
            _companiesModel->itemAppended();
        }
    }
}

void AssociateWidget::handleRequestAssociationsFinished()
{
    _waitingDialog->cancel();
    if (_reply2 != NULL) {
        _reply2->close();
        _reply2->deleteLater();

        QByteArray json = _reply2->readAll();
        _reply2 = NULL;
        QJson::Parser parser;
        bool ok;
        QVariantList companies = parser.parse (json, &ok).toList();
        if (!ok) {
            qDebug() << "some error happend on server";
            return;
        }
        for (QVariantList::iterator i = companies.begin(); i != companies.end(); ++i) {
            Company company = Company::fromJson(*i);
            int idx = _companies.indexOf(company);
            if (idx != -1) {
                _companies.removeAt(idx);
                _companiesModel->refresh();
            }
            _associations.append(company);
            _associationsModel->itemAppended();
        }
    }
}

