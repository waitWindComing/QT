#include "issueeditor.h"
#include "issuerule.h"
#include "issueruletablemodel.h"
#include "issueruletabledelegate.h"
#include <QHeaderView>
#include <QMessageBox>
#include <qDebug>

IssueEditor::IssueEditor(const qulonglong &managerId, const QString &managerName, const int &level, const QString &token, const qulonglong &companyId, const QString &companyName, const double &latitude, const double &longitude, const QList<IssueRule *> &issueRules, qlonglong netPay, QWidget *parent) :
    _managerId(managerId),
    _managerName(managerName),
    _level(level),
    _token(token),
    _companyId(companyId),
    _companyName(companyName),
    _latitude(latitude),
    _longitude(longitude),
    _netPay(netPay),
    QWidget(parent)
{
    setStyleSheet("background: #c08585");

    // only show valid rules
    for (QList<IssueRule *>::const_iterator i = issueRules.begin(); i != issueRules.end(); ++i) {
        (*i)->setCheckState(Qt::Unchecked);
        if (_netPay >= (*i)->getStartingAmount()) {
            _issueRules.append(*i);
        }
    }
    if (!_issueRules.isEmpty()) {
        qSort(_issueRules.begin(), _issueRules.end(), ruleLessThan);
        _issueRules.first()->setCheckState(Qt::Checked);
    }
    _model = new IssueRuleTableModel(_issueRules, this);
    _rulesView = new QTableView(this);
    _rulesView->setModel(_model);
    _ruleDelegate = new IssueRuleTableDelegate();
    _rulesView->setItemDelegate(_ruleDelegate);
    _rulesView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _rulesView->horizontalHeader()->setVisible(true);
    _rulesView->verticalHeader()->setVisible(false);
    _rulesView->setGeometry(0, 0, 1024 - 250, 540 - 32);
    _rulesView->setColumnWidth(0, 60);
    _rulesView->setColumnWidth(1, 350);
    _rulesView->setColumnWidth(2, 60);
    _rulesView->setColumnWidth(3, 130);
    _rulesView->setColumnWidth(4, 100);
    _rulesView->setColumnWidth(5, 55);
    _rulesView->verticalHeader()->setDefaultSectionSize(55);
}

IssueEditor::~IssueEditor()
{
    delete _ruleDelegate;
}

IssueRule *IssueEditor::getSelectedIssueRule()
{
    IssueRule *selectedRule = NULL;
    for (QList<IssueRule *>::iterator i = _issueRules.begin(); i != _issueRules.end(); ++i) {
        if ((*i)->getCheckState() == Qt::Checked) {
            selectedRule = *i;
            break;
        }
    }
    return selectedRule;
}

bool IssueEditor::ruleLessThan(IssueRule *r1, IssueRule *r2)
{
    return r1->getStartingAmount() > r2->getStartingAmount();
}

