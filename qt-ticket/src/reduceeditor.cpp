#include "reduceeditor.h"
#include "availableticketmodel.h"
#include "availableticketdelegate.h"
#include <QHeaderView>
#include <qDebug>

ReduceEditor::ReduceEditor(const qulonglong &managerId, const QString &managerName, const int &level, const QString &token, const qulonglong &companyId, const QString &companyName, const double &latitude, const double &longitude, const QList<Ticket *> &tickets, const QList<UseRule *> &useRules, QWidget *parent) :
    _managerId(managerId),
    _managerName(managerName),
    _level(level),
    _token(token),
    _companyId(companyId),
    _companyName(companyName),
    _latitude(latitude),
    _longitude(longitude),
    _tickets(tickets),
    _useRules(useRules),
    QWidget(parent)
{
    _ticketsView = new QTableView(this);
    _model = new AvailableTicketModel(_tickets, this);
    _ticketsView->setModel(_model);
    _delegate = new AvailableTicketDelegate(_companyId, _useRules);
    _ticketsView->setItemDelegate(_delegate);
    _ticketsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ticketsView->horizontalHeader()->setVisible(true);
    _ticketsView->verticalHeader()->setVisible(false);
    _ticketsView->setGeometry(0, 0, 1024 - 250, 540 - 32);
    _ticketsView->setColumnWidth(0, 100);
    _ticketsView->setColumnWidth(1, 300);
    _ticketsView->setColumnWidth(2, 55);
    _ticketsView->setColumnWidth(3, 100);
    _ticketsView->setColumnWidth(4, 65);
    _ticketsView->setColumnWidth(5, 65);
    _ticketsView->setColumnWidth(6, 55);
    _ticketsView->verticalHeader()->setDefaultSectionSize(55);
    connect(_delegate, SIGNAL(editingInputted(bool)), this, SIGNAL(editingInputted(bool)));
}

ReduceEditor::~ReduceEditor()
{
    delete _delegate;
}

void ReduceEditor::showEvent ( QShowEvent * event )
{
    _model->refresh();
}


