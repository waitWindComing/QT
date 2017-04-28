#include "consumewidget.h"
#include "companycustomer.h"
#include "customer.h"
#include "ticketmodel.h"
#include "ticketusemodel.h"
#include <QLabel>
#include <QTableView>
#include <QHeaderView>
#include <qDebug>

ConsumeWidget::ConsumeWidget(Consume *consume, QWidget *parent)
    : _consume(consume),
    QWidget(parent)
{
    QLabel *customerLabel = new QLabel(QString("%1%2").arg(trUtf8("顾客号码：")).arg(consume->getCompanyCustomer()->getCustomer()->getName()), this);
    customerLabel->setGeometry(10, 10, 150, 30);
    QLabel *amountLabel = new QLabel(QString("%1%2").arg(trUtf8("应收金额：")).arg(consume->getAmount() / 100.0, 0, 'f', 2), this);
    amountLabel->setGeometry(180, 10, 120, 30);

    qlonglong netPaid = consume->getNetPaid();
    qlonglong netDeduction = consume->getNetDeduction();
    if (netPaid != 0) {
        QLabel *netLabel = new QLabel(QString("%1%2").arg(trUtf8("实收金额：")).arg(netPaid / 100.0, 0, 'f', 2), this);
        netLabel->setGeometry(330, 10, 120, 30);
    }
    if (netDeduction != 0) {
        QLabel *netLabel = new QLabel(QString("%1%2").arg(trUtf8("实扣金额：")).arg(netDeduction / 100.0, 0, 'f', 2), this);
        netLabel->setGeometry(330, 10, 120, 30);
    }
    QLabel *consumeWhenLabel = new QLabel(QString("%1%2").arg(trUtf8("操作时间：")).arg(consume->getConsumeWhen().toString("yyyy-M-d h:m:s")), this);
    consumeWhenLabel->setGeometry(480, 10, 180, 30);

    QLabel *acceptLabel = new QLabel(QString("%1%2%3").arg(trUtf8("本次收券：")).arg(consume->getTicketUses().size()).arg(trUtf8("张")), this);
    acceptLabel->setGeometry(10, 40, 150, 30);

    QTableView *acceptTable = new QTableView(this);
    TicketUseModel *ticketUseModel = new TicketUseModel(consume->getTicketUses(), this);
    acceptTable->setModel(ticketUseModel);
    acceptTable->horizontalHeader()->setVisible(true);
    acceptTable->verticalHeader()->setVisible(false);
    acceptTable->setColumnWidth(0, 90);
    acceptTable->setColumnWidth(1, 90);
    acceptTable->setColumnWidth(2, 240);
    acceptTable->setColumnWidth(3, 240);
    acceptTable->setColumnWidth(4, 90);
    acceptTable->setGeometry(10, 70, 1024 - 250, 250);
    acceptTable->show();

    QLabel *issueLabel = new QLabel(QString("%1%2%3").arg(trUtf8("本次发券：")).arg(consume->getTickets().size()).arg(trUtf8("张")), this);
    issueLabel->setGeometry(10, 330, 150, 30);

    QTableView *issueTable = new QTableView(this);
    TicketModel *ticketModel = new TicketModel(consume->getTickets(), this);
    issueTable->setModel(ticketModel);
    issueTable->horizontalHeader()->setVisible(true);
    issueTable->verticalHeader()->setVisible(false);
    issueTable->setColumnWidth(0, 90);
    issueTable->setColumnWidth(1, 90);
    issueTable->setColumnWidth(2, 480);
    issueTable->setColumnWidth(3, 110);
    issueTable->setGeometry(10, 360, 1024 - 250, 180);
    issueTable->show();

}


