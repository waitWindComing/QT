#include "prepayresultdlg.h"
#include "consume.h"
#include "companycustomer.h"
#include "customer.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDialogButtonBox>

PrepayResultDlg::PrepayResultDlg(const Consume * const consume, QWidget *parent)
    : _autoCloseCount(10),
      QDialog(parent)
{
    setWindowTitle(trUtf8("支付结果"));
    _timer = new QTimer(this);
    QVBoxLayout *lt = new QVBoxLayout;

    QHBoxLayout *idLayout = new QHBoxLayout;
    idLayout->addWidget(new QLabel(trUtf8("用户号码："), this));
    idLayout->addWidget(new QLabel(QString("%1").arg(consume->getCompanyCustomer()->getCustomer()->getName()), this));
    lt->addLayout(idLayout);
    
    QHBoxLayout *preLayout = new QHBoxLayout;
    preLayout->addWidget(new QLabel(trUtf8("原余额："), this));
    preLayout->addWidget(new QLabel(QString("%1").arg(consume->getPreBalance() / 100.0, 0, 'f', 2), this));
    lt->addLayout(preLayout);
    
    QHBoxLayout *amountLayout = new QHBoxLayout;
    amountLayout->addWidget(new QLabel(trUtf8("本次消费："), this));
    amountLayout->addWidget(new QLabel(QString("%1").arg(consume->getNetDeduction() / 100.0, 0, 'f', 2), this));
    lt->addLayout(amountLayout);
    
    QHBoxLayout *postLayout = new QHBoxLayout;
    postLayout->addWidget(new QLabel(trUtf8("新余额："), this));
    postLayout->addWidget(new QLabel(QString("%1").arg(consume->getPostBalance() / 100.0, 0, 'f', 2), this));
    lt->addLayout(postLayout);
    
    _okButton = new QPushButton(trUtf8("返回(10)"), this);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal, this);
    buttonBox->addButton(_okButton, QDialogButtonBox::AcceptRole);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));

    lt->addWidget(buttonBox);
    setLayout(lt);

    connect(_timer, SIGNAL(timeout()), this, SLOT(tictac()));
    _timer->start(1000);
}

void PrepayResultDlg::tictac()
{
    _autoCloseCount--;
    if (_autoCloseCount >= 0) {
        _okButton->setText(trUtf8("返回") + QString("(%1)").arg(_autoCloseCount));
    } else {
        accept();
    }
}

