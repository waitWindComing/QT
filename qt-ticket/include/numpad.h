#ifndef NUM_PAD_H
#define NUM_PAD_H

#include <QWidget>
#include <QPushButton>
#include <QSignalMapper>

class NumPad : public QWidget
{
    Q_OBJECT

public:
    NumPad(QWidget *parent);

signals:
    void characterGenerated(QChar c);
    void delClicked();
    void productClicked();

private slots:
    void saveFocusWidget(QWidget *oldFocus, QWidget *newFocus);
    void zeroClicked();
    void oneClicked();
    void twoClicked();
    void threeClicked();
    void fourClicked();
    void fiveClicked();
    void sixClicked();
    void sevenClicked();
    void eightClicked();
    void nineClicked();
    void dotClicked();
    void divClicked();
    void multiClicked();
    void addClicked();
    void subClicked();
    void enterClicked();
    void handleDelClicked();
    void handleProductClicked();

private:
    QPushButton *_zeroButton;
    QPushButton *_oneButton;
    QPushButton *_twoButton;
    QPushButton *_threeButton;
    QPushButton *_fourButton;
    QPushButton *_fiveButton;
    QPushButton *_sixButton;
    QPushButton *_sevenButton;
    QPushButton *_eightButton;
    QPushButton *_nineButton;
    QPushButton *_dotButton;
    QPushButton *_delButton;
    QPushButton *_divButton;
    QPushButton *_multiButton;
    QPushButton *_addButton;
    QPushButton *_subButton;
    QPushButton *_enterButton;
    QPushButton *_productButton;

    QWidget *_lastFocusedWidget;
};

#endif

