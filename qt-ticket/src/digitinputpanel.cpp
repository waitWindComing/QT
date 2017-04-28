#include "digitinputpanel.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>

DigitInputPanel::DigitInputPanel(QWidget *parent)
    : QWidget(parent),
      _lastFocusedWidget(0)
{
    connect(qApp, SIGNAL(focusChanged(QWidget*,QWidget*)), this, SLOT(saveFocusWidget(QWidget*,QWidget*)));

    _zeroButton = new QPushButton("0", this);
    _zeroButton->setFixedSize(130, 60);
    _oneButton = new QPushButton("1", this);
    _oneButton->setFixedSize(60, 60);
    _twoButton = new QPushButton("2", this);
    _twoButton->setFixedSize(60, 60);
    _threeButton = new QPushButton("3", this);
    _threeButton->setFixedSize(60, 60);
    _fourButton = new QPushButton("4", this);
    _fourButton->setFixedSize(60, 60);
    _fiveButton = new QPushButton("5", this);
    _fiveButton->setFixedSize(60, 60);
    _sixButton = new QPushButton("6", this);
    _sixButton->setFixedSize(60, 60);
    _sevenButton = new QPushButton("7", this);
    _sevenButton->setFixedSize(60, 60);
    _eightButton = new QPushButton("8", this);
    _eightButton->setFixedSize(60, 60);
    _nineButton = new QPushButton("9", this);
    _nineButton->setFixedSize(60, 60);
    _delButton = new QPushButton("Del", this);
    _delButton->setFixedSize(60, 60);

    QVBoxLayout *lt = new QVBoxLayout;
    lt->setSpacing(0);
    lt->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *lineOneLayout = new QHBoxLayout;
    lineOneLayout->setContentsMargins(0, 0, 0, 0);
    lineOneLayout->setSpacing(0);
    lineOneLayout->addWidget(_oneButton);
    lineOneLayout->addSpacing(10);
    lineOneLayout->addWidget(_twoButton);
    lineOneLayout->addSpacing(10);
    lineOneLayout->addWidget(_threeButton);
    lt->addLayout(lineOneLayout);
    lt->addSpacing(10);

    QHBoxLayout *lineTwoLayout = new QHBoxLayout;
    lineTwoLayout->setContentsMargins(0, 0, 0, 0);
    lineTwoLayout->setSpacing(0);
    lineTwoLayout->addWidget(_fourButton);
    lineTwoLayout->addSpacing(10);
    lineTwoLayout->addWidget(_fiveButton);
    lineTwoLayout->addSpacing(10);
    lineTwoLayout->addWidget(_sixButton);
    lt->addLayout(lineTwoLayout);
    lt->addSpacing(10);

    QHBoxLayout *lineThreeLayout = new QHBoxLayout;
    lineThreeLayout->setContentsMargins(0, 0, 0, 0);
    lineThreeLayout->setSpacing(0);
    lineThreeLayout->addWidget(_sevenButton);
    lineThreeLayout->addSpacing(10);
    lineThreeLayout->addWidget(_eightButton);
    lineThreeLayout->addSpacing(10);
    lineThreeLayout->addWidget(_nineButton);
    lt->addLayout(lineThreeLayout);
    lt->addSpacing(10);

    QHBoxLayout *lineFourLayout = new QHBoxLayout;
    lineFourLayout->setContentsMargins(0, 0, 0, 0);
    lineFourLayout->setSpacing(0);
    lineFourLayout->addWidget(_zeroButton);
    lineFourLayout->addSpacing(10);
    lineFourLayout->addWidget(_delButton);
    lt->addLayout(lineFourLayout);

    setLayout(lt);

    connect(_zeroButton, SIGNAL(clicked()), this, SLOT(zeroClicked()));
    connect(_oneButton, SIGNAL(clicked()), this, SLOT(oneClicked()));
    connect(_twoButton, SIGNAL(clicked()), this, SLOT(twoClicked()));
    connect(_threeButton, SIGNAL(clicked()), this, SLOT(threeClicked()));
    connect(_fourButton, SIGNAL(clicked()), this, SLOT(fourClicked()));
    connect(_fiveButton, SIGNAL(clicked()), this, SLOT(fiveClicked()));
    connect(_sixButton, SIGNAL(clicked()), this, SLOT(sixClicked()));
    connect(_sevenButton, SIGNAL(clicked()), this, SLOT(sevenClicked()));
    connect(_eightButton, SIGNAL(clicked()), this, SLOT(eightClicked()));
    connect(_nineButton, SIGNAL(clicked()), this, SLOT(nineClicked()));
    connect(_delButton, SIGNAL(clicked()), this, SLOT(delClicked()));
}

void DigitInputPanel::saveFocusWidget(QWidget * /*oldFocus*/, QWidget *newFocus) 
{
    if (newFocus != 0 && !this->isAncestorOf(newFocus)) {
        _lastFocusedWidget = newFocus;
    }
}

void DigitInputPanel::zeroClicked()
{
    if (_lastFocusedWidget)
        _lastFocusedWidget->setFocus();
    emit characterGenerated('0');
}

void DigitInputPanel::oneClicked()
{
    if (_lastFocusedWidget)
        _lastFocusedWidget->setFocus();
    emit characterGenerated('1');
}

void DigitInputPanel::twoClicked()
{
    if (_lastFocusedWidget)
        _lastFocusedWidget->setFocus();
    emit characterGenerated('2');
}

void DigitInputPanel::threeClicked()
{
    if (_lastFocusedWidget)
        _lastFocusedWidget->setFocus();
    emit characterGenerated('3');
}

void DigitInputPanel::fourClicked()
{
    if (_lastFocusedWidget)
        _lastFocusedWidget->setFocus();
    emit characterGenerated('4');
}

void DigitInputPanel::fiveClicked()
{
    if (_lastFocusedWidget)
        _lastFocusedWidget->setFocus();
    emit characterGenerated('5');
}

void DigitInputPanel::sixClicked()
{
    if (_lastFocusedWidget)
        _lastFocusedWidget->setFocus();
    emit characterGenerated('6');
}

void DigitInputPanel::sevenClicked()
{
    if (_lastFocusedWidget)
        _lastFocusedWidget->setFocus();
    emit characterGenerated('7');
}

void DigitInputPanel::eightClicked()
{
    if (_lastFocusedWidget)
        _lastFocusedWidget->setFocus();
    emit characterGenerated('8');
}

void DigitInputPanel::nineClicked()
{
    if (_lastFocusedWidget)
        _lastFocusedWidget->setFocus();
    emit characterGenerated('9');
}

void DigitInputPanel::delClicked()
{
    if (_lastFocusedWidget)
        _lastFocusedWidget->setFocus();
    emit characterGenerated(127);
}

