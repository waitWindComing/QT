#ifndef DIGIT_INPUT_PANEL_H
#define DIGIT_INPUT_PANEL_H

#include <QWidget>
#include <QPushButton>
#include <QSignalMapper>

class DigitInputPanel : public QWidget
{
    Q_OBJECT

public:
    DigitInputPanel(QWidget *parent);

signals:
    void characterGenerated(QChar c);

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
    void delClicked();

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
    QPushButton *_delButton;

    QWidget *_lastFocusedWidget;
};

#endif

