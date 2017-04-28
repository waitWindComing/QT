#include "issuedticketdlg.h"
#include "ticket.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDialogButtonBox>

IssuedTicketDlg::IssuedTicketDlg(const Ticket * const ticket, QWidget *parent)
    : _ticket(ticket),
      _autoCloseCount(10),
      QDialog(parent)
{
    setWindowTitle(trUtf8("已发券"));
    _timer = new QTimer(this);
    QVBoxLayout *lt = new QVBoxLayout;

    QHBoxLayout *idLayout = new QHBoxLayout;
    idLayout->addWidget(new QLabel(trUtf8("号码:"), this));
    idLayout->addWidget(new QLabel(QString("%1").arg(ticket->getTicketId()), this));
    lt->addLayout(idLayout);

    if (Ticket::Discount == ticket->getType()) {
        QHBoxLayout *typeLayout = new QHBoxLayout;
        typeLayout->addWidget(new QLabel(trUtf8("类型:"), this));
        typeLayout->addWidget(new QLabel(trUtf8("折扣券"), this));
        lt->addLayout(typeLayout);

        QHBoxLayout *onceLayout = new QHBoxLayout;
        onceLayout->addWidget(new QLabel(trUtf8("是否一次性:"), this));
        if (ticket->getOnce()) {
            onceLayout->addWidget(new QLabel(trUtf8("一次性"), this));
        } else {
            onceLayout->addWidget(new QLabel(trUtf8("反复使用"), this));
        }
        lt->addLayout(onceLayout);

        QHBoxLayout *discountLayout = new QHBoxLayout;
        discountLayout->addWidget(new QLabel(trUtf8("折扣率:"), this));
        discountLayout->addWidget(new QLabel(QString("%1%").arg(ticket->getDiscount()), this));
        lt->addLayout(discountLayout);

    } else {
        QHBoxLayout *typeLayout = new QHBoxLayout;
        typeLayout->addWidget(new QLabel(trUtf8("类型:"), this));
        typeLayout->addWidget(new QLabel(trUtf8("代金券"), this));
        lt->addLayout(typeLayout);

        QHBoxLayout *deductionLayout = new QHBoxLayout;
        deductionLayout->addWidget(new QLabel(trUtf8("抵扣金额:"), this));
        deductionLayout->addWidget(new QLabel(QString("%1").arg(ticket->getDeduction() / 100.0, 0, 'f', 2), this));
        lt->addLayout(deductionLayout);
    }
    
    QHBoxLayout *expiredLayout = new QHBoxLayout;
    expiredLayout->addWidget(new QLabel(trUtf8("有效期:"), this));
    QDateTime expiredWhen = ticket->getExpiredWhen();
    expiredLayout->addWidget(new QLabel(expiredWhen.toString("yyyy-M-d h:m:s"), this));
    lt->addLayout(expiredLayout);

    _okButton = new QPushButton(trUtf8("返回(10)"), this);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal, this);
    buttonBox->addButton(_okButton, QDialogButtonBox::AcceptRole);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));

    lt->addWidget(buttonBox);
    setLayout(lt);

    connect(_timer, SIGNAL(timeout()), this, SLOT(tictac()));
    _timer->start(1000);
}

void IssuedTicketDlg::tictac()
{
    _autoCloseCount--;
    if (_autoCloseCount >= 0) {
        _okButton->setText(trUtf8("返回") + QString("(%1)").arg(_autoCloseCount));
    } else {
        accept();
    }
}

