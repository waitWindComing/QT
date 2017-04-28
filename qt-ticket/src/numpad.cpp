#include "numpad.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>

NumPad::NumPad(QWidget *parent)
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
    _dotButton = new QPushButton(".", this);
    _dotButton->setFixedSize(60, 60);
    _delButton = new QPushButton("<-", this);
    _delButton->setFixedSize(60, 60);
    _divButton = new QPushButton("/", this);
    _divButton->setFixedSize(60, 60);
    _multiButton = new QPushButton("*", this);
    _multiButton->setFixedSize(60, 60);
    _subButton = new QPushButton("-", this);
    _subButton->setFixedSize(60, 60);
    _addButton = new QPushButton("+", this);
    _addButton->setFixedSize(60, 60);
    _enterButton = new QPushButton("Enter", this);
    _enterButton->setFixedSize(60, 130);
    _enterButton->setStyleSheet("font-size: 14px;");
    _productButton = new QPushButton("P", this);
    _productButton->setFixedSize(60, 60);

    QHBoxLayout *lt = new QHBoxLayout;
    lt->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *lineOneLayout = new QHBoxLayout;
    lineOneLayout->addWidget(_delButton);
    lineOneLayout->addSpacing(10);
    lineOneLayout->addWidget(_divButton);
    lineOneLayout->addSpacing(10);
    lineOneLayout->addWidget(_multiButton);
    leftLayout->addLayout(lineOneLayout);

    QHBoxLayout *lineTwoLayout = new QHBoxLayout;
    lineTwoLayout->addWidget(_sevenButton);
    lineTwoLayout->addSpacing(10);
    lineTwoLayout->addWidget(_eightButton);
    lineTwoLayout->addSpacing(10);
    lineTwoLayout->addWidget(_nineButton);
    leftLayout->addLayout(lineTwoLayout);

    QHBoxLayout *lineThreeLayout = new QHBoxLayout;
    lineThreeLayout->addWidget(_fourButton);
    lineThreeLayout->addSpacing(10);
    lineThreeLayout->addWidget(_fiveButton);
    lineThreeLayout->addSpacing(10);
    lineThreeLayout->addWidget(_sixButton);
    leftLayout->addLayout(lineThreeLayout);

    QHBoxLayout *lineFourLayout = new QHBoxLayout;
    lineFourLayout->addWidget(_oneButton);
    lineFourLayout->addSpacing(10);
    lineFourLayout->addWidget(_twoButton);
    lineFourLayout->addSpacing(10);
    lineFourLayout->addWidget(_threeButton);
    leftLayout->addLayout(lineFourLayout);

    QHBoxLayout *lineFiveLayout = new QHBoxLayout;
    lineFiveLayout->addWidget(_zeroButton);
    lineFiveLayout->addSpacing(10);
    lineFiveLayout->addWidget(_dotButton);
    leftLayout->addLayout(lineFiveLayout);

    rightLayout->addWidget(_subButton);
    rightLayout->addSpacing(10);
    rightLayout->addWidget(_addButton);
    rightLayout->addSpacing(10);
    rightLayout->addWidget(_productButton);
    rightLayout->addSpacing(10);
    rightLayout->addWidget(_enterButton);

    lt->addLayout(leftLayout);
    lt->addLayout(rightLayout);

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
    connect(_dotButton, SIGNAL(clicked()), this, SLOT(dotClicked()));
    connect(_delButton, SIGNAL(clicked()), this, SLOT(handleDelClicked()));
    connect(_divButton, SIGNAL(clicked()), this, SLOT(divClicked()));
    connect(_multiButton, SIGNAL(clicked()), this, SLOT(multiClicked()));
    connect(_addButton, SIGNAL(clicked()), this, SLOT(addClicked()));
    connect(_subButton, SIGNAL(clicked()), this, SLOT(subClicked()));
    connect(_enterButton, SIGNAL(clicked()), this, SLOT(enterClicked()));
    connect(_productButton, SIGNAL(clicked()), this, SLOT(handleProductClicked()));
}

void NumPad::saveFocusWidget(QWidget * /*oldFocus*/, QWidget *newFocus) 
{
    if (newFocus != 0 && !this->isAncestorOf(newFocus)) {
        _lastFocusedWidget = newFocus;
    }
}

void NumPad::zeroClicked()
{
    if (_lastFocusedWidget)
        _lastFocusedWidget->setFocus();
    emit characterGenerated('0');
}

void NumPad::oneClicked()
{
    if (_lastFocusedWidget)
        _lastFocusedWidget->setFocus();
    emit characterGenerated('1');
}

void NumPad::twoClicked()
{
    if (_lastFocusedWidget)
        _lastFocusedWidget->setFocus();
    emit characterGenerated('2');
}

void NumPad::threeClicked()
{
    if (_lastFocusedWidget)
        _lastFocusedWidget->setFocus();
    emit characterGenerated('3');
}

void NumPad::fourClicked()
{
    if (_lastFocusedWidget)
        _lastFocusedWidget->setFocus();
    emit characterGenerated('4');
}

void NumPad::fiveClicked()
{
    if (_lastFocusedWidget)
        _lastFocusedWidget->setFocus();
    emit characterGenerated('5');
}

void NumPad::sixClicked()
{
    if (_lastFocusedWidget)
        _lastFocusedWidget->setFocus();
    emit characterGenerated('6');
}

void NumPad::sevenClicked()
{
    if (_lastFocusedWidget)
        _lastFocusedWidget->setFocus();
    emit characterGenerated('7');
}

void NumPad::eightClicked()
{
    if (_lastFocusedWidget)
        _lastFocusedWidget->setFocus();
    emit characterGenerated('8');
}

void NumPad::nineClicked()
{
    if (_lastFocusedWidget)
        _lastFocusedWidget->setFocus();
    emit characterGenerated('9');
}

void NumPad::dotClicked()
{
    if (_lastFocusedWidget)
        _lastFocusedWidget->setFocus();
    emit characterGenerated('.');
}

void NumPad::handleDelClicked()
{
    if (_lastFocusedWidget)
        _lastFocusedWidget->setFocus();
    emit delClicked();
}

void NumPad::divClicked()
{
    if (_lastFocusedWidget)
        _lastFocusedWidget->setFocus();
    emit characterGenerated('/');
}

void NumPad::multiClicked()
{
    if (_lastFocusedWidget)
        _lastFocusedWidget->setFocus();
    emit characterGenerated('*');
}

void NumPad::addClicked()
{
    if (_lastFocusedWidget)
        _lastFocusedWidget->setFocus();
    emit characterGenerated('+');
}

void NumPad::subClicked()
{
    if (_lastFocusedWidget)
        _lastFocusedWidget->setFocus();
    emit characterGenerated('-');
}

void NumPad::enterClicked()
{
    if (_lastFocusedWidget)
        _lastFocusedWidget->setFocus();
    emit characterGenerated(13);
}

void NumPad::handleProductClicked()
{
    if (_lastFocusedWidget)
        _lastFocusedWidget->setFocus();
    emit productClicked();
}

