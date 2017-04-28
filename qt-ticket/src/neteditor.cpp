#include "neteditor.h"
#include "ticket.h"
#include "useticketmodel.h"
#include "useticketdelegate.h"
#include "useruletablemodel.h"
#include "useruletabledelegate.h"
#include <QScrollArea>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <qDebug>

NetEditor::NetEditor(const qulonglong &managerId, const QString &managerName, const int &level, const QString &token, const qulonglong &companyId, const QString &companyName, const double &latitude, const double &longitude, const QList<Ticket *> &selectedTickets, const QList<UseRule *> &useRules, qlonglong due, QWidget *parent) :
    _managerId(managerId),
    _managerName(managerName),
    _level(level),
    _token(token),
    _companyId(companyId),
    _companyName(companyName),
    _latitude(latitude),
    _longitude(longitude),
    _tickets(selectedTickets),
    _useRules(useRules),
    _due(due),
    _net(due),
    QWidget(parent)
{
    if (!_useRules.isEmpty()) {
        _rulesView = new QTableView(this);
        _rulesView->setModel(new UseRuleTableModel(_useRules, this));
        _ruleDelegate = new UseRuleTableDelegate(this);
        _rulesView->setItemDelegate(_ruleDelegate);
        _rulesView->setSelectionBehavior(QAbstractItemView::SelectRows);
        _rulesView->horizontalHeader()->setVisible(true);
        _rulesView->verticalHeader()->setVisible(false);
        _rulesView->setGeometry(0, 0, 1024 - 250, (540 - 32) / 2);
        _rulesView->setColumnWidth(0, 380);
        _rulesView->setColumnWidth(1, 60);
        _rulesView->setColumnWidth(2, 60);
        _rulesView->setColumnWidth(3, 80);
        _rulesView->setColumnWidth(4, 120);
        _rulesView->setColumnWidth(5, 50);
        _rulesView->verticalHeader()->setDefaultSectionSize(55);
        connect(_ruleDelegate, SIGNAL(ruleChanged()), this, SLOT(updateSummary()));
    }

    _ticketsView = new QTableView(this);
    _ticketsView->setModel(new UseTicketModel(_tickets, due, this));
    _ticketDelegate = new UseTicketDelegate(this);
    _ticketsView->setItemDelegate(_ticketDelegate);
    _ticketsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ticketsView->horizontalHeader()->setVisible(true);
    _ticketsView->verticalHeader()->setVisible(false);
    if (_useRules.isEmpty()) {
        _ticketsView->setGeometry(0, 0, 1024 - 250, 540 - 32);
    } else {
        _ticketsView->setGeometry(0, (540 - 32) / 2, 1024 - 250, (540 - 32) / 2);
    }
    _ticketsView->setColumnWidth(0, 100);
    _ticketsView->setColumnWidth(1, 100);
    _ticketsView->setColumnWidth(2, 120);
    _ticketsView->setColumnWidth(3, 150);
    _ticketsView->setColumnWidth(4, 120);
    _ticketsView->setColumnWidth(5, 65);
    _ticketsView->setColumnWidth(6, 55);
    _ticketsView->verticalHeader()->setDefaultSectionSize(55);
    connect(_ticketDelegate, SIGNAL(editingInputted()), this, SLOT(updateSummary()));

    for (QList<Ticket *>::iterator i = _tickets.begin(); i != _tickets.end(); ++i) {
        if (Ticket::Groupon == (*i)->getType()){
            if ((*i)->getCheckState() == Qt::Checked) {
                (*i)->setSaveAfterUse((*i)->getLeftDeduction() - (*i)->getCost());
                (*i)->setLeftDeductionAfterUse(0);
                (*i)->setValidAfterUse(false);
            } else {
                (*i)->setValidAfterUse(true);
                (*i)->setSaveAfterUse(0);
                (*i)->setLeftDeductionAfterUse((*i)->getLeftDeduction());
            }
        }
    }
}

NetEditor::~NetEditor()
{
}

void NetEditor::showEvent ( QShowEvent * event )
{
    qDebug() << "NetEditor::showEvent()";
    updateSummary();
}

void NetEditor::updateSummary()
{
    UseRule *useRule = NULL;
    for (QList<UseRule *>::iterator i = _useRules.begin(); i != _useRules.end(); ++i) {
        if ((*i)->getCheckState() == Qt::Checked) {
            useRule = *i;
            break;
        }
    }

    if (isGroupon()) {
        int useup = 0;
        qlonglong cost = 0;
        qlonglong deduction = 0;
        for (QList<Ticket *>::iterator i = _tickets.begin(); i != _tickets.end(); ++i) {
            if (!(*i)->getValidAfterUse()) {
                ++useup;
                cost += (*i)->getCost();
                deduction += (*i)->getLeftDeduction();
            }
        }
        if (deduction >= _due) {
            _net = cost;
        } else {
            _net = _due - deduction + cost;
        }
        qlonglong save = _due - _net;
        QString info = trUtf8("应付: %1元, 实付: %2元, 其中团购费：%3元，补交：%4元，共节省%5元");
        info = info.arg(_due / 100.0, 0, 'f', 2).arg(_net / 100.0, 0, 'f', 2).arg(cost / 100.0, 0, 'f', 2).arg((_net - cost)/ 100.0, 0, 'f', 2).arg(save / 100.0, 0, 'f', 2);
        emit summaryUpdated(info);
    } else {
        UseTicketModel *model = (UseTicketModel *)_ticketsView->model();
        qlonglong save = model->calSaveWithUseRule(useRule);
        int useup = 0;
        for (QList<Ticket *>::iterator i = _tickets.begin(); i != _tickets.end(); ++i) {
            if (!(*i)->getValidAfterUse()) {
                ++useup;
            }
        }
        _net = _due - save;
        QString info = trUtf8("本次消费应付: %1元, 实付: %2元, 一共用掉%3张优惠券, 共节省%4元");
        info = info.arg(_due / 100.0, 0, 'f', 2).arg((_net) / 100.0, 0, 'f', 2).arg(useup).arg(save / 100.0, 0, 'f', 2);
        emit summaryUpdated(info);
    }
}

qlonglong NetEditor::getNet()
{
    return _net;
}

UseRule *NetEditor::getSelectedUseRule()
{
    UseRule *useRule = NULL;
    for (QList<UseRule *>::iterator i = _useRules.begin(); i != _useRules.end(); ++i) {
        if ((*i)->getCheckState() == Qt::Checked) {
            useRule = *i;
            break;
        }
    }
    return useRule;
}

bool NetEditor::isGroupon()
{
    bool rst = false;
    for (QList<Ticket *>::iterator i = _tickets.begin(); i != _tickets.end(); ++i) {
        rst = (*i)->getType() == Ticket::Groupon;
        break;
    }
    return rst;
}

