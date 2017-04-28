#ifndef PREPAY_RESULT_DLG_H
#define PREPAY_RESULT_DLG_H

#include <QDialog>
#include <QTimer>
#include <QPushButton>

class Consume;
class PrepayResultDlg : public QDialog
{
    Q_OBJECT

public:
    explicit PrepayResultDlg(const Consume * const consume, QWidget *parent = 0);

public slots:
    void tictac();

private:
    QTimer *_timer;
    QPushButton *_okButton;
    int _autoCloseCount;
};

#endif

