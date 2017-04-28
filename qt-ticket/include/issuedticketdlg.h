#ifndef ISSUED_TICKET_DLG_H
#define ISSUED_TICKET_DLG_H

#include <QDialog>
#include <QTimer>
#include <QPushButton>

class Ticket;
class IssuedTicketDlg : public QDialog
{
    Q_OBJECT

public:
    explicit IssuedTicketDlg(const Ticket * const ticket, QWidget *parent = 0);

public slots:
    void tictac();

private:
    QTimer *_timer;
    QPushButton *_okButton;
    int _autoCloseCount;
    const Ticket * const _ticket;
};

#endif

