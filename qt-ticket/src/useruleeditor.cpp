#include "useruleeditor.h"
#include "userule.h"
#include "usedeductionrule.h"
#include "usediscountrule.h"
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
#include <QDoubleValidator>
#include <QJson/Serializer>
#include <QJson/Parser>
#include <qDebug>

UseRuleEditor::UseRuleEditor(const qulonglong &managerId, const QString &managerName, const int &level, const QString &token, const qulonglong &companyId, const QString &companyName, const double &latitude, const double &longitude, UseRule *rule, QWidget *parent) :
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
        _issuedByCompanies = rule->getIssuedByCompanies();
    } else {
        _ruleId = 0;
    }

    setStyleSheet("background: #484848");

    QDoubleValidator *amountValidator = new QDoubleValidator(0, 999999, 2, this);
    amountValidator->setNotation(QDoubleValidator::StandardNotation);

    QLabel *nameLabel = new QLabel(trUtf8("名称"), this);
    nameLabel->setGeometry(40, 20, 50, 30);
    rule ? _nameEdit = new QLineEdit(rule->getRuleName(), this) : _nameEdit = new QLineEdit(this);
    _nameEdit->setGeometry(100, 20, 360, 30);

    QLabel *leastLabel = new QLabel(trUtf8("最少消费(元)"), this);
    leastLabel->setGeometry(40, 60, 75, 30);
    rule ? _leastConsumeEdit = new QLineEdit(QString::number(rule->getLeastConsume() / 100.0, 'f', 2), this) : _leastConsumeEdit = new QLineEdit(this);
    _leastConsumeEdit->setGeometry(120, 60, 80, 30);
    _leastConsumeEdit->setValidator(amountValidator);

    QLabel *maxLabel = new QLabel(trUtf8("最多抵扣(元)"), this);
    maxLabel->setGeometry(240, 60, 75, 30);
    rule ? _maxDeductionEdit = new QLineEdit(QString::number(rule->getMaxDeduction() / 100.0, 'f', 2), this) : _maxDeductionEdit = new QLineEdit(this);
    _maxDeductionEdit->setGeometry(320, 60, 80, 30);
    _maxDeductionEdit->setValidator(amountValidator);

    QLabel *typeLabel = new QLabel(trUtf8("类型"), this);
    typeLabel->setGeometry(40, 100, 50, 30);
    _typeGroup = new QButtonGroup(this);
    _discountRadio = new QRadioButton(trUtf8("折扣券"), this);
    _deductionRadio = new QRadioButton(trUtf8("代金券"), this);
    _discountRadio->setGeometry(100, 100, 80, 30);
    _deductionRadio->setGeometry(200, 100, 80, 30);
    _typeGroup->addButton(_discountRadio);
    _typeGroup->addButton(_deductionRadio);
    _typeGroup->setId(_discountRadio, 0);
    _typeGroup->setId(_deductionRadio, 1);
    connect(_typeGroup, SIGNAL(buttonClicked(int)), this, SLOT(on_typeGroup_clicked(int)));

    QValidator *discountValidator = new QIntValidator(1, 100, this);
    _rateLabel = new QLabel(trUtf8("抵扣率(%)"), this);
    _rateLabel->setGeometry(40, 140, 70, 30);
    rule ? _rateEdit = new QLineEdit(QString::number(rule->getRate()), this) : _rateEdit = new QLineEdit(this);
    _rateEdit->setGeometry(110, 140, 50, 30);
    _rateEdit->setValidator(discountValidator);

    _discountTypeGroup = new QButtonGroup(this);
    _oldDiscountRadio = new QRadioButton(trUtf8("券折扣*兑换系数"), this);
    _newDiscountRadio = new QRadioButton(trUtf8("新折扣率"), this);
    _oldDiscountRadio->setGeometry(40, 140, 120, 30);
    _newDiscountRadio->setGeometry(170, 140, 80, 30);
    _discountTypeGroup->addButton(_oldDiscountRadio);
    _discountTypeGroup->addButton(_newDiscountRadio);
    _discountTypeGroup->setId(_oldDiscountRadio, 0);
    _discountTypeGroup->setId(_newDiscountRadio, 1);
    connect(_discountTypeGroup, SIGNAL(buttonClicked(int)), this, SLOT(on_discountTypeGroup_clicked(int)));

    _convertRateLabel = new QLabel(trUtf8("兑换系数"), this);
    _convertRateLabel->setGeometry(270, 140, 75, 30);
    _convertRateLabel->hide();
    rule ? _convertRateEdit = new QLineEdit(QString::number(rule->getConvertRate()), this) : _convertRateEdit = new QLineEdit(QString::number(1.0), this);
    _convertRateEdit->setGeometry(350, 140, 50, 30);
    _convertRateEdit->hide();

    _discountLabel = new QLabel(trUtf8("新折扣率(%)"), this);
    _discountLabel->setGeometry(270, 140, 80, 30);
    _discountLabel->hide();
    rule ? _discountEdit = new QLineEdit(QString::number(rule->getDiscount()), this) : _discountEdit = new QLineEdit(this);
    _discountEdit->setGeometry(360, 140, 50, 30);
    _discountEdit->setValidator(discountValidator);
    _discountEdit->hide();

    QLabel *companiesLabel = new QLabel(trUtf8("适用商户"), this);
    companiesLabel->setGeometry(40, 180, 60, 30);
    _companyType = new QComboBox(this);
    QStringList companyTypes;
    companyTypes << trUtf8("自身") << trUtf8("选择特定商户");
    _companyType->addItems(companyTypes);
    _companyType->setGeometry(110, 180, 150, 30);
    QListView *listView = new QListView(_companyType);
    _companyType->setView(listView);

    connect(_companyType, SIGNAL(currentIndexChanged(int)), this, SLOT(on_companyType_Changed(int)));

    _companiesList = new QListView(this);
    _companiesList->setGeometry(40, 220, 450, 310);
    _companiesList->hide();

    _model = new CompanyModel(_associatedCompanies);
    _companiesList->setModel(_model);

    if (rule == NULL) {
        setMode(Editable);
    } else {
        QList<Company> issuedByCompanies = rule->getIssuedByCompanies();
        if (issuedByCompanies.size() == 0) {
            _companyType->setCurrentIndex(0);
        } else {
            _companyType->setCurrentIndex(1);
        }
        setMode(ReadOnly);
    }

    if (rule) {
        if (rule->getType() == UseRule::Discount) {
            _discountRadio->setChecked(true);
            changeRuleType(0);
            if (rule->getDiscountType() == 0) {
                _oldDiscountRadio->setChecked(true);
                changeDiscountType(0);
            } else {
                _newDiscountRadio->setChecked(true);
                changeDiscountType(1);
            }
        } else {
            _deductionRadio->setChecked(true);
            changeRuleType(1);
        }
    } else {
        _deductionRadio->setChecked(true);
        changeRuleType(1);
    }
}

UseRuleEditor::~UseRuleEditor()
{
}

void UseRuleEditor::setMode(Mode mode)
{
    if (mode == ReadOnly) {
        _nameEdit->setReadOnly(true);
        _leastConsumeEdit->setReadOnly(true);
        _maxDeductionEdit->setReadOnly(true);
        _discountRadio->setEnabled(false);
        _deductionRadio->setEnabled(false);
        _rateEdit->setReadOnly(true);
        _oldDiscountRadio->setEnabled(false);
        _newDiscountRadio->setEnabled(false);
        _discountEdit->setReadOnly(true);
        _convertRateEdit->setReadOnly(true);
        _companyType->setEnabled(false);
        _companiesList->setSelectionMode(QAbstractItemView::NoSelection);
    } else {
        _nameEdit->setReadOnly(false);
        _leastConsumeEdit->setReadOnly(false);
        _maxDeductionEdit->setReadOnly(false);
        _discountRadio->setEnabled(true);
        _deductionRadio->setEnabled(true);
        _rateEdit->setReadOnly(false);
        _discountEdit->setReadOnly(false);
        _oldDiscountRadio->setEnabled(true);
        _newDiscountRadio->setEnabled(true);
        _discountEdit->setReadOnly(false);
        _convertRateEdit->setReadOnly(false);
        _companyType->setEnabled(true);
        _companiesList->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }
}

void UseRuleEditor::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void UseRuleEditor::on_typeGroup_clicked(int id)
{
    qDebug() << "on_typeGroup_clicked: " << id;
    changeRuleType(id);
}

void UseRuleEditor::on_discountTypeGroup_clicked(int id)
{
    qDebug() << "on_discountTypeGroup_clicked: " << id;
    changeDiscountType(id);
}

void UseRuleEditor::changeRuleType(int id)
{
    if (id == 0) {   // discount
        if (_oldDiscountRadio->isChecked()) {
            changeDiscountType(0);
        } else {
            changeDiscountType(1);
        }
        _oldDiscountRadio->show();
        _newDiscountRadio->show();
        _rateLabel->hide();
        _rateEdit->hide();
    } else if (id == 1) {   // deduction
        _rateLabel->show();
        _rateEdit->show();
        _oldDiscountRadio->setChecked(true);
        _oldDiscountRadio->hide();
        _newDiscountRadio->hide();
        _discountLabel->hide();
        _discountEdit->hide();
        _convertRateLabel->hide();
        _convertRateEdit->hide();
    }
}

void UseRuleEditor::changeDiscountType(int id)
{
    if (id == 0) {   // old
        _discountLabel->hide();
        _discountEdit->hide();
        _convertRateLabel->show();
        _convertRateEdit->show();
    } else if (id == 1) {   // new discount
        _discountLabel->show();
        _discountEdit->show();
        _convertRateLabel->hide();
        _convertRateEdit->hide();
    }
}

void UseRuleEditor::on_companyType_Changed(int index)
{
    qDebug() << "UseRuleEditor::on_companyType_Changed";
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

void UseRuleEditor::saveRule()
{
    qDebug() << "UseRuleEditor::saveRule()";
    if (_ruleId == 0) {    // POST
        QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/useRules")).arg(_managerId)));
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
        QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/useRules/%2")).arg(_managerId).arg(_ruleId)));
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

bool UseRuleEditor::inputToJson(QVariantMap &variant)
{
    variant.insert("name", _nameEdit->text());
    bool ok;
    qlonglong leastConsume = _leastConsumeEdit->text().toDouble(&ok) * 100;
    if (!ok) {
        qDebug() << "leastConsume input error";
        return ok;
    }
    variant.insert("leastConsume", leastConsume);
    qlonglong maxDeduction = _maxDeductionEdit->text().toDouble(&ok) * 100;
    if (!ok) {
        qDebug() << "maxDeduction input error";
        return ok;
    }
    variant.insert("maxDeduction", maxDeduction);
    if (_typeGroup->checkedId() == 0) {
        if (_discountTypeGroup->checkedId() == 0) {
            variant.insert("discountType", 0);
            double convertRate = _convertRateEdit->text().toDouble(&ok);
            if (!ok) {
                qDebug() << "convertRate input error";
                return ok;
            }
            variant.insert("convertRate", convertRate);
        } else {
            variant.insert("discountType", 1);
            int discount = _discountEdit->text().toInt(&ok, 10);
            if (!ok) {
                qDebug() << "discount input error";
                return ok;
            }
            variant.insert("discount", discount);
        }
    } else {
        int rate = _rateEdit->text().toInt(&ok, 10);
        if (!ok) {
            qDebug() << "rate input error";
            return ok;
        }
        variant.insert("rate", rate);
    }

    int companyType = _companyType->currentIndex();
    if (companyType == 1) {
        QItemSelectionModel *selectionModel = _companiesList->selectionModel();
        QItemSelection itemSelection = selectionModel->selection();
        QModelIndexList selectedIndexes = itemSelection.indexes();
        QVariantList issuedByCompanies;
        for (QModelIndexList::iterator i = selectedIndexes.begin(); i != selectedIndexes.end(); ++i) {
            QVariantMap issuedByCompany;
            Company selectedCompany = _associatedCompanies.at((*i).row());
            issuedByCompany.insert("id", selectedCompany.getCompanyId());
            issuedByCompany.insert("name", selectedCompany.getName());
            issuedByCompany.insert("latitude", selectedCompany.getLatitude());
            issuedByCompany.insert("longitude", selectedCompany.getLongitude());
            issuedByCompanies.append(issuedByCompany);
        }
        variant.insert("issuedByCompanies", issuedByCompanies);
    }
    return true;
}

void UseRuleEditor::delRule()
{
    QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/useRules/%2")).arg(_managerId).arg(_ruleId)));
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

void UseRuleEditor::handleDeleteRuleFinished()
{
    if (_reply != NULL) {
        _reply->close();
        _reply->deleteLater();
        if (_reply->error() == QNetworkReply::NoError) {
            emit ruleDeleted(_ruleId);
        } else if (_reply->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
        } 
    }
}

void UseRuleEditor::handlePutRuleFinished()
{
    if (_reply != NULL) {
        _reply->close();
        _reply->deleteLater();
        if (_reply->error() == QNetworkReply::NoError) {
            QByteArray json = _reply->readAll();
            QJson::Parser parser;
            bool ok;
            QVariant var = parser.parse (json, &ok);
            UseRule *rule = UseRule::fromJson(var);
            emit rulePutted(_ruleId, rule);
        } else if (_reply->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
        } 
    }
}

void UseRuleEditor::handlePostRuleFinished()
{
    if (_reply != NULL) {
        _reply->close();
        _reply->deleteLater();
        if (_reply->error() == QNetworkReply::NoError) {
            QByteArray json = _reply->readAll();
            QJson::Parser parser;
            bool ok;
            QVariant var = parser.parse (json, &ok);
            UseRule *rule = UseRule::fromJson(var);
            emit rulePosted(rule);
        } else if (_reply->error() == QNetworkReply::AuthenticationRequiredError) {
            qDebug() << "authentication error";
        } 
    }
}

void UseRuleEditor::requestAssociateCompanies()
{
    qDebug() << "UseRuleEditor::requestAssociateCompanies";
    QNetworkRequest request(QUrl((Utils::getHost().append("/managers/%1/associateCompanies")).arg(_managerId)));
    request.setRawHeader(QByteArray("X-AUTH-TOKEN"), _token.toUtf8());
    _reply = _qnam.get(request);
    connect(_reply, SIGNAL(finished()), this, SLOT(handleRequestAssociateCompaniesFinished()));
}

void UseRuleEditor::handleRequestAssociateCompaniesFinished()
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
            QVariantMap var = (*i).toMap();
            qulonglong companyId = var["id"].toULongLong();
            QString companyName = var["name"].toString();
            //QString location = var["location"].toString();
            double latitude = var["latitude"].toDouble();
            double longitude = var["longitude"].toDouble();
            Company associateCompany(companyId, companyName, latitude, longitude);
            _associatedCompanies.append(associateCompany);
            _model->itemAppended();

            bool found = false;
            for (QList<Company>::iterator j = _issuedByCompanies.begin(); j != _issuedByCompanies.end(); ++j) {
                Company issuedByCompany = *j;
                if (issuedByCompany == associateCompany) {
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

