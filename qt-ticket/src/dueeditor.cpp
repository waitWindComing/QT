#include "dueeditor.h"
#include "numpad.h"
#include "numcontext.h"
#include "muParser.h"
#include <QLabel>
#include <QStyleOption>
#include <QPainter>
#include <QApplication>
#include <QRegExpValidator>
#include <QDoubleValidator>
#include <qDebug>

DueEditor::DueEditor(const qulonglong &managerId, const QString &managerName, const int &level, const QString &token, const qulonglong &companyId, const QString &companyName, const double &latitude, const double &longitude, QWidget *parent) :
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
    QLabel *customerLabel = new QLabel(trUtf8("用户号码"), this);
    customerLabel->setGeometry(40, 80, 60, 30);
    _customerEdit = new QLineEdit(this);
    _customerEdit->setInputMask("999-9999-9999");
    _customerEdit->setFont(QFont("Timers", 24));
    _customerEdit->setGeometry(40, 120, 260, 60);
    connect(_customerEdit, SIGNAL(textChanged(const QString &)), this, SLOT(handleTextChanged(const QString &)));

    QLabel *dueLabel = new QLabel(trUtf8("应付金额(元)"), this);
    dueLabel->setGeometry(40, 240, 80, 30);
    _dueEdit = new QLineEdit(this);
    _dueEdit->setFont(QFont("Timers", 24));
    _dueEdit->setGeometry(40, 280, 340, 60);
    connect(_dueEdit, SIGNAL(textChanged(const QString &)), this, SLOT(handleTextChanged(const QString &)));
    connect(_dueEdit, SIGNAL(returnPressed()), this, SLOT(handleDueReturnPressed()));

    NumPad *inputPanel = new NumPad(this);
    inputPanel->setGeometry(450, 80, 300, 340);
    NumContext *ic = new NumContext(inputPanel, this);
    _customerEdit->setInputContext(ic);
    _dueEdit->setInputContext(ic);

    connect(inputPanel, SIGNAL(productClicked()), this, SLOT(handleProductSelection()));
    connect(inputPanel, SIGNAL(delClicked()), this, SLOT(handleDelClicked()));
}

DueEditor::~DueEditor()
{
}

void DueEditor::focus()
{
    _customerEdit->setFocus();
}

QString DueEditor::getCustomer()
{
    return _customerEdit->text().remove('-');
}

qlonglong DueEditor::getDue()
{
    return _dueEdit->text().toDouble() * 100;
}

QMap<CompanyProduct, int> DueEditor::getSelectedCompanyProducts()
{
    return _selectedCompanyProducts;
}

void DueEditor::clear()
{
    _customerEdit->clear();
    _dueEdit->clear();
    _customerEdit->setFocus();
    _selectedCompanyProducts.clear();
}

void DueEditor::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void DueEditor::handleTextChanged(const QString &text)
{
    QString customer = _customerEdit->text();
    QString due = _dueEdit->text();
    int pos = 0;
    QRegExpValidator *custValidator = new QRegExpValidator(QRegExp("1\\d{2}-\\d{4}-\\d{4}"), this);
    QDoubleValidator *dueValidator = new QDoubleValidator(0, 999999, 2, this);
    if (QValidator::Acceptable == custValidator->validate(customer, pos) &&  QValidator::Acceptable == dueValidator->validate(due, pos)) {
        emit editingFinished();
    } else {
        emit editingUnFinished();
    }
    delete custValidator;
    delete dueValidator;
}

void DueEditor::handleDueReturnPressed()
{
    QString due = _dueEdit->text();
    try {
        mu::Parser p;
        p.SetExpr(due.toStdString());
        double result = p.Eval();
        due = QString("%1").arg(result, 0, 'f', 2);
        if (due.endsWith(".00")) {
            due.remove(".00");
        }
        _dueEdit->setText(due);
    } catch (mu::Parser::exception_type &e) {
    }
}

void DueEditor::handleProductSelection()
{
    QString customerName = getCustomer();
    ProductSelectionDlg dlg(_managerId, _companyId, _token, _selectedCompanyProducts, customerName, this);
    if (dlg.exec() == QDialog::Accepted) {
        qlonglong cost = 0;
        QMap<CompanyProduct, int> selectedCompanyProducts = dlg.getSelectedCompanyProducts();
        QMapIterator<CompanyProduct, int> it(selectedCompanyProducts);
        while(it.hasNext()) {
            it.next();
            if (_selectedCompanyProducts.contains(it.key())) {
                _selectedCompanyProducts.insert(it.key(), it.value() + _selectedCompanyProducts.value(it.key()));
            } else {
                _selectedCompanyProducts.insert(it.key(), it.value());
            }
            CompanyProductPrice price = it.key().getCurrentPrice();
            cost += price.getPrice() * it.value();
        }
        if (cost == 0) {
            return;
        }
        QString due = _dueEdit->text();
        if (due.endsWith("+")) {
            due = QString("%1%2").arg(due).arg(cost / 100.0, 0, 'f', 2);
        } else {
            due = QString("%1+%2").arg(due).arg(cost / 100.0, 0, 'f', 2);
        }
        _dueEdit->setText(due);
    }
}

void DueEditor::handleDelClicked()
{
    // remove end charator or remove all if companyProduct list is not empty
    if (_dueEdit->hasFocus()) {
        if (_selectedCompanyProducts.empty()) {
            QString due = _dueEdit->text();
            due.chop(1);
            _dueEdit->setText(due);
        } else {
            _selectedCompanyProducts.clear();
            _dueEdit->setText(QString());
        }
    } else if (_customerEdit->hasFocus()) {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
        QApplication::sendEvent(_customerEdit, &keyPress);

        QKeyEvent keyRelease(QEvent::KeyRelease, Qt::Key_Backspace, Qt::NoModifier);
        QApplication::sendEvent(_customerEdit, &keyRelease);
    }
}

