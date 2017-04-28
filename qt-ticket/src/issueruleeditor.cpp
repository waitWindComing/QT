#include "issueruleeditor.h"
#include "issuerule.h"
#include "issuedeductionrule.h"
#include "issuediscountrule.h"
#include "companymodel.h"
#include "utils.h"
#include <QStyleOption>
#include <QPainter>
#include <QLabel>
#include <QRadioButton>
#include <QLineEdit>
#include <QButtonGroup>
#include <QComboBox>
#include <QListView>
#include <QItemSelectionModel>
#include <QItemSelection>
#include <QIntValidator>
#include <QMessageBox>
#include <QDoubleValidator>
#include <QJson/Serializer>
#include <QJson/Parser>
#include <qDebug>

IssueRuleEditor::IssueRuleEditor(const qulonglong &managerId, const QString &managerName, const int &level, const QString &token, const qulonglong &companyId, const QString &companyName, const double &latitude, const double &longitude, IssueRule *rule, QWidget *parent) :
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
    if (rule) {
        _ruleId = rule->getRuleId();
        _validCompanies = rule->getValidCompanies();
    } else {
        _ruleId = 0;
    }

    setStyleSheet("background: #484848");

    QLabel *nameLabel = new QLabel(trUtf8("名称"), this);
    nameLabel->setGeometry(40, 20, 50, 30);
    rule ? _nameEdit = new QLineEdit(rule->getRuleName(), this) : _nameEdit = new QLineEdit(this);
    _nameEdit->setGeometry(100, 20, 360, 30);

    QLabel *typeLabel = new QLabel(trUtf8("类型"), this);
    typeLabel->setGeometry(40, 60, 50, 30);
    _typeGroup = new QButtonGroup(this);
    _discountRadio = new QRadioButton(trUtf8("折扣券"), this);
    _deductionRadio = new QRadioButton(trUtf8("代金券"), this);
    _discountRadio->setGeometry(100, 60, 80, 30);
    _deductionRadio->setGeometry(200, 60, 80, 30);
    _typeGroup->addButton(_discountRadio);
    _typeGroup->addButton(_deductionRadio);
    _typeGroup->setId(_discountRadio, 0);
    _typeGroup->setId(_deductionRadio, 1);
    connect(_typeGroup, SIGNAL(buttonClicked(int)), this, SLOT(on_typeGroup_clicked(int)));

    QLabel *expiredLabel = new QLabel(trUtf8("有效期"), this);
    expiredLabel->setGeometry(300, 60, 50, 30);
    _expiredCombo = new QComboBox(this);
    QStringList expiredStrs;
    expiredStrs << trUtf8("一个月") << trUtf8("两个月") << trUtf8("三个月") << trUtf8("半年") << trUtf8("一年") << trUtf8("两年") << trUtf8("三年") << trUtf8("永远有效");
    _expiredCombo->addItems(expiredStrs);
    _expiredCombo->setGeometry(360, 60, 120, 30);
    QListView *listView = new QListView(_expiredCombo);
    _expiredCombo->setView(listView);
    setExpiredCurrentIndex(rule);

    QLabel *startingAmountLabel = new QLabel(trUtf8("满"), this);
    startingAmountLabel->setGeometry(40, 100, 20, 30);
    rule ? _startingAmountEdit = new QLineEdit(QString::number(rule->getStartingAmount() / 100.0, 'f', 2), this) : _startingAmountEdit = new QLineEdit(this);
    _startingAmountEdit->setGeometry(60, 100, 80, 30);
    QDoubleValidator *amountValidator = new QDoubleValidator(0, 999999, 2, this);
    amountValidator->setNotation(QDoubleValidator::StandardNotation);
    _startingAmountEdit->setValidator(amountValidator);

    _startingDeductionLabel = new QLabel(trUtf8("元返"), this);
    _startingDeductionLabel->setGeometry(140, 100, 30, 30);
    rule ? _startingDeductionEdit = new QLineEdit(QString::number(rule->getStartingDeduction() / 100.0, 'f', 2), this) : _startingDeductionEdit = new QLineEdit(this);
    _startingDeductionEdit->setGeometry(170, 100, 80, 30);
    _startingDeductionEdit->setValidator(amountValidator);

    _stepAmountLabel = new QLabel(trUtf8("元，之后每"), this);
    _stepAmountLabel->setGeometry(250, 100, 70, 30);
    rule ? _stepAmountEdit = new QLineEdit(QString::number(rule->getStepAmount() / 100.0, 'f', 2), this) : _stepAmountEdit = new QLineEdit(this);
    _stepAmountEdit->setGeometry(320, 100, 80, 30);
    _stepAmountEdit->setValidator(amountValidator);

    _stepDeductionLabel = new QLabel(trUtf8("元返"), this);
    _stepDeductionLabel->setGeometry(400, 100, 30, 30);
    rule ? _stepDeductionEdit = new QLineEdit(QString::number(rule->getStepDeduction() / 100.0, 'f', 2), this) : _stepDeductionEdit = new QLineEdit(this);
    _stepDeductionEdit->setGeometry(430, 100, 80, 30);
    _stepDeductionEdit->setValidator(amountValidator);

    _discountLabel = new QLabel(trUtf8("元，折扣率(%)"), this);
    _discountLabel->setGeometry(140, 100, 90, 30);
    _discountLabel->hide();
    rule ? _discountEdit = new QLineEdit(QString::number(rule->getDiscount()), this) : _discountEdit = new QLineEdit(this);
    _discountEdit->setGeometry(230, 100, 50, 30);
    _discountEdit->hide();
    QValidator *discountValidator = new QIntValidator(1, 99, this);
    _discountEdit->setValidator(discountValidator);

    _discountOnceLabel = new QLabel(trUtf8("折扣方式"), this);
    _discountOnceLabel->setGeometry(290, 100, 50, 30);
    _discountOnceLabel->hide();
    _discountGroup = new QButtonGroup(this);
    _onceYesRadio = new QRadioButton(trUtf8("一次性"), this);
    _onceNoRadio = new QRadioButton(trUtf8("反复使用"), this);
    _onceYesRadio->setGeometry(340, 100, 60, 30);
    _onceNoRadio->setGeometry(400, 100, 80, 30);
    _onceYesRadio->hide();
    _onceNoRadio->hide();
    _discountGroup->addButton(_onceYesRadio);
    _discountGroup->addButton(_onceNoRadio);
    _discountGroup->setId(_onceYesRadio, 0);
    _discountGroup->setId(_onceNoRadio, 1);

    QLabel *companiesLabel = new QLabel(trUtf8("适用商户"), this);
    companiesLabel->setGeometry(40, 140, 60, 30);
    _companyType = new QComboBox(this);
    QStringList companyTypes;
    companyTypes << trUtf8("自身") << trUtf8("选择特定商户");
    _companyType->addItems(companyTypes);
    _companyType->setGeometry(110, 140, 150, 30);
    listView = new QListView(_companyType);
    _companyType->setView(listView);

    connect(_companyType, SIGNAL(currentIndexChanged(int)), this, SLOT(on_companyType_Changed(int)));

    _companiesList = new QListView(this);
    _companiesList->setGeometry(40, 180, 450, 310);
    _companiesList->hide();

    _model = new CompanyModel(_associatedCompanies);
    _companiesList->setModel(_model);

    if (rule == NULL) {
        setMode(Editable);
    } else {
        QList<Company> validCompanies = rule->getValidCompanies();
        if (validCompanies.size() == 0) {
            _companyType->setCurrentIndex(0);
        } else {
            _companyType->setCurrentIndex(1);
        }
        setMode(ReadOnly);
    }

    if (rule) {
        if (rule->getType() == IssueRule::Discount) {
            _discountRadio->setChecked(true);
            if (rule->getOnce()) {
                _onceYesRadio->setChecked(true);
            } else {
                _onceNoRadio->setChecked(true);
            }
            changeIssueType(0);
        } else {
            _deductionRadio->setChecked(true);
            _onceYesRadio->setChecked(true);
            changeIssueType(1);
        }
    } else {
        _deductionRadio->setChecked(true);
        _onceYesRadio->setChecked(true);
        changeIssueType(1);
    }
}

IssueRuleEditor::~IssueRuleEditor()
{
}

void IssueRuleEditor::setMode(Mode mode)
{
    if (mode == ReadOnly) {
        _nameEdit->setReadOnly(true);
        _startingAmountEdit->setReadOnly(true);
        _expiredCombo->setEnabled(false);
        _discountRadio->setEnabled(false);
        _deductionRadio->setEnabled(false);
        _startingDeductionEdit->setReadOnly(true);
        _stepAmountEdit->setReadOnly(true);
        _stepDeductionEdit->setReadOnly(true);
        _discountEdit->setReadOnly(true);
        _onceYesRadio->setEnabled(false);
        _onceNoRadio->setEnabled(false);
        _companyType->setEnabled(false);
        _companiesList->setSelectionMode(QAbstractItemView::NoSelection);
    } else {
        _nameEdit->setReadOnly(false);
        _startingAmountEdit->setReadOnly(false);
        _expiredCombo->setEnabled(true);
        _discountRadio->setEnabled(true);
        _deductionRadio->setEnabled(true);
        _startingDeductionEdit->setReadOnly(false);
        _stepAmountEdit->setReadOnly(false);
        _stepDeductionEdit->setReadOnly(false);
        _discountEdit->setReadOnly(false);
        _onceYesRadio->setEnabled(true);
        _onceNoRadio->setEnabled(true);
        _companyType->setEnabled(true);
        _companiesList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }
}

void IssueRuleEditor::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void IssueRuleEditor::on_typeGroup_clicked(int id)
{
    qDebug() << "on_typeGroup_clicked: " << id;
    changeIssueType(id);
}

void IssueRuleEditor::changeIssueType(int id)
{
    if (id == 0) {   // discount
        _discountLabel->show();
        _discountEdit->show();
        _discountOnceLabel->show();
        _onceYesRadio->show();
        _onceNoRadio->show();
        _startingDeductionLabel->hide();
        _startingDeductionEdit->hide();
        _stepAmountLabel->hide();
        _stepAmountEdit->hide();
        _stepDeductionLabel->hide();
        _stepDeductionEdit->hide();
    } else if (id == 1) {   // deduction
        _startingDeductionLabel->show();
        _startingDeductionEdit->show();
        _stepAmountLabel->show();
        _stepAmountEdit->show();
        _stepDeductionLabel->show();
        _stepDeductionEdit->show();
        _discountLabel->hide();
        _discountEdit->hide();
        _discountOnceLabel->hide();
        _onceYesRadio->hide();
        _onceNoRadio->hide();
    }
}

void IssueRuleEditor::on_companyType_Changed(int index)
{
    qDebug() << "IssueRuleEditor::on_companyType_Changed";
    switch (index) {
    case 1:
        if (_associatedCompanies.size() == 0) {
            requestAssociateCompanies();
        }
        _companiesList->show();
        break;
    default:
        _companiesList->hide();
        break;
    }
}

void IssueRuleEditor::saveRule()
{
    qDebug() << "IssueRuleEditor::saveRule()";
    if (_ruleId == 0) {    // POST
        QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/issueRules")).arg(_managerId)));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
        QVariantMap variant;
        bool ok;
        ok = inputToJson(variant);
        if (!ok) {
            qDebug() << "convert to Json error, Please check input again.";
            return;
        }
        QJson::Serializer serializer;
        QByteArray data = serializer.serialize(variant, &ok);
        Q_ASSERT(ok);
        _reply = _qnam.post(request, data);
        connect(_reply, SIGNAL(finished()), this, SLOT(handlePostRuleFinished()));
    } else {    // PUT
        QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/issueRules/%2")).arg(_managerId).arg(_ruleId)));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());

        QVariantMap variant;
        bool ok;
        ok = inputToJson(variant);
        if (!ok) {
            qDebug() << "convert to Json error, Please check input again.";
            return;
        }
        QJson::Serializer serializer;
        QByteArray data = serializer.serialize(variant, &ok);
        Q_ASSERT(ok);
        _reply = _qnam.put(request, data);
        connect(_reply, SIGNAL(finished()), this, SLOT(handlePutRuleFinished()));
    }
}

bool IssueRuleEditor::inputToJson(QVariantMap &variant)
{
    variant.insert("name", _nameEdit->text());
    bool ok;
    int startingAmount = _startingAmountEdit->text().toDouble(&ok) * 100;
    if (!ok) {
        qDebug() << "startingAmount input error";
        return ok;
    }
    variant.insert("startingAmount", startingAmount);
    variant.insert("expiredMonths", getExpiredMonths());
    if (_typeGroup->checkedId() == 0) {   // discount
        int discount = _discountEdit->text().toInt(&ok, 10);
        if (!ok) {
            qDebug() << "discount input error";
            return ok;
        }
        variant.insert("discount", discount);
        if (_discountGroup->checkedId() == 0) {
            variant.insert("once", true);
        } else {
            variant.insert("once", false);
        }
    } else {  // deduction
        qlonglong startingDeduction = _startingDeductionEdit->text().toDouble(&ok) * 100;
        if (!ok) {
            qDebug() << "startingDeduction input error";
            return ok;
        }
        variant.insert("startingDeduction", startingDeduction);
        qlonglong stepAmount = _stepAmountEdit->text().toDouble(&ok) * 100;
        if (!ok || stepAmount <= 0) {
            QMessageBox msgBox;
            msgBox.setText(trUtf8("返券间隔金额必须大于0！"));
            msgBox.exec();
            _stepAmountEdit->setFocus();
            return false;
        }
        variant.insert("stepAmount", stepAmount);
        qlonglong stepDeduction = _stepDeductionEdit->text().toDouble(&ok) * 100;
        if (!ok) {
            qDebug() << "stepDeduction input error";
            return ok;
        }
        variant.insert("stepDeduction", stepDeduction);
    }

    int companyType = _companyType->currentIndex();
    if (companyType == 1) {
        QItemSelectionModel *selectionModel = _companiesList->selectionModel();
        QItemSelection itemSelection = selectionModel->selection();
        QModelIndexList selectedIndexes = itemSelection.indexes();
        QVariantList validCompanies;
        for (QModelIndexList::iterator i = selectedIndexes.begin(); i != selectedIndexes.end(); ++i) {
            QVariantMap validCompany;
            Company selectedCompany = _associatedCompanies.at((*i).row());
            validCompany.insert("id", selectedCompany.getCompanyId());
            validCompany.insert("name", selectedCompany.getName());
            validCompany.insert("latitude", selectedCompany.getLatitude());
            validCompany.insert("longitude", selectedCompany.getLongitude());
            validCompanies.append(validCompany);
        }
        variant.insert("validCompanies", validCompanies);
    }
    return true;
}

void IssueRuleEditor::delRule()
{
    QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/issueRules/%2")).arg(_managerId).arg(_ruleId)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());

    QVariantMap variant;
    variant.insert("isEnabled", false);

    QJson::Serializer serializer;
    bool ok;
    QByteArray data = serializer.serialize(variant, &ok);
    Q_ASSERT(ok);
    _reply = _qnam.put(request, data);
    connect(_reply, SIGNAL(finished()), this, SLOT(handleDeleteRuleFinished()));
}

void IssueRuleEditor::handleDeleteRuleFinished()
{
    if (_reply != NULL) {
        _reply->close();
        _reply->deleteLater();
        if (_reply->error() == QNetworkReply::NoError) {
            emit ruleDeleted(_ruleId);
        } else if (_reply->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
            QMessageBox::critical(this, trUtf8("认证失败"), trUtf8("请退出系统重新认证！"));
        } 
    }
}

void IssueRuleEditor::handlePutRuleFinished()
{
    if (_reply != NULL) {
        _reply->close();
        _reply->deleteLater();
        if (_reply->error() == QNetworkReply::NoError) {
            QByteArray json = _reply->readAll();
            QJson::Parser parser;
            bool ok;
            QVariant var = parser.parse (json, &ok);
            IssueRule *rule = IssueRule::fromJson(var);
            emit rulePutted(_ruleId, rule);
        } else if (_reply->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
            QMessageBox::critical(this, trUtf8("认证失败"), trUtf8("请退出系统重新认证！"));
        } 
    }
}

void IssueRuleEditor::handlePostRuleFinished()
{
    if (_reply != NULL) {
        _reply->close();
        _reply->deleteLater();
        if (_reply->error() == QNetworkReply::NoError) {
            QByteArray json = _reply->readAll();
            QJson::Parser parser;
            bool ok;
            QVariant var = parser.parse (json, &ok);
            IssueRule *rule = IssueRule::fromJson(var);
            emit rulePosted(rule);
        } else if (_reply->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
            QMessageBox::critical(this, trUtf8("认证失败"), trUtf8("请退出系统重新认证！"));
        } 
    }
}

void IssueRuleEditor::setExpiredCurrentIndex(IssueRule *rule)
{
    if (rule != NULL) {
        switch (rule->getExpiredMonths()) {
        case 0:
            _expiredCombo->setCurrentIndex(7);
            break;
        case 1:
            _expiredCombo->setCurrentIndex(0);
            break;
        case 2:
            _expiredCombo->setCurrentIndex(1);
            break;
        case 3:
            _expiredCombo->setCurrentIndex(2);
            break;
        case 6:
            _expiredCombo->setCurrentIndex(3);
            break;
        case 12:
            _expiredCombo->setCurrentIndex(4);
            break;
        case 24:
            _expiredCombo->setCurrentIndex(5);
            break;
        case 36:
            _expiredCombo->setCurrentIndex(6);
            break;
        default:
            break;
        }
    } else {
        _expiredCombo->setCurrentIndex(3);
    }
}

int IssueRuleEditor::getExpiredMonths()
{
    int months = 0;
    switch(_expiredCombo->currentIndex()) {
    case 0:
        months = 1;
        break;
    case 1:
        months = 2;
        break;
    case 2:
        months = 3;
        break;
    case 3:
        months = 6;
        break;
    case 4:
        months = 12;
        break;
    case 5:
        months = 24;
        break;
    case 6:
        months = 36;
        break;
    case 7:
        months = 0;   // 永远有效
        break;
    default:
        break;
    }
    return months;
}

void IssueRuleEditor::requestAssociateCompanies()
{
    qDebug() << "IssueRuleEditor::requestAssociateCompanies";
    QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/associateCompanies")).arg(_managerId)));
    request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
    _reply = _qnam.get(request);
    connect(_reply, SIGNAL(finished()), this, SLOT(handleRequestAssociateCompaniesFinished()));
}

void IssueRuleEditor::handleRequestAssociateCompaniesFinished()
{
    if (_reply != NULL) {
        _reply->close();
        _reply->deleteLater();

        QByteArray json = _reply->readAll();
        QJson::Parser parser;
        bool ok;
        QVariantList companies = parser.parse (json, &ok).toList();
        if (!ok) {
            qDebug() << "some error happend on server";
            return;
        }
        QItemSelection selection;
        for (QVariantList::iterator i = companies.begin(); i != companies.end(); ++i) {
            Company associateCompany = Company::fromJson(*i);
            _associatedCompanies.append(associateCompany);
            _model->itemAppended();

            bool found = false;
            for (QList<Company>::iterator j = _validCompanies.begin(); j != _validCompanies.end(); ++j) {
                Company validCompany = *j;
                if (validCompany == associateCompany) {
                    found = true;
                    break;
                }
            }
            if (found) {
                QModelIndex curIndex = _model->index(_associatedCompanies.size() - 1, _associatedCompanies.size() - 1, QModelIndex());
                selection.select(curIndex, curIndex);
            }
        }
        QItemSelectionModel *selectionModel = _companiesList->selectionModel();
        selectionModel->select(selection, QItemSelectionModel::Select);
    }
}

