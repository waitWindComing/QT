#include "digitinputcontext.h"
#include "digitinputpanel.h"
#include <QPointer>
#include <QApplication>

DigitInputContext::DigitInputContext(DigitInputPanel *inputPanel, QObject *parent)
  :_inputPanel(inputPanel),
   QInputContext(parent)
{
    connect(_inputPanel, SIGNAL(characterGenerated(QChar)), SLOT(sendCharacter(QChar)));
}

QString DigitInputContext::identifierName()
{
    return "DigitInputContext";
}

void DigitInputContext::reset()
{
}

bool DigitInputContext::isComposing() const
{
    return false;
}

QString DigitInputContext::language()
{
    return "en_US";
}

void DigitInputContext::sendCharacter(QChar character)
{ 
    QPointer<QWidget> w = focusWidget();
 
    if (!w)                   
        return;               
 
    if (character == 127) {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
        QApplication::sendEvent(w, &keyPress);

        QKeyEvent keyRelease(QEvent::KeyRelease, Qt::Key_Backspace, Qt::NoModifier);
        QApplication::sendEvent(w, &keyRelease);
    } else {
        QKeyEvent keyPress(QEvent::KeyPress, character.unicode(), Qt::NoModifier, QString(character));
        QApplication::sendEvent(w, &keyPress);

        QKeyEvent keyRelease(QEvent::KeyRelease, character.unicode(), Qt::NoModifier, QString(character));
        QApplication::sendEvent(w, &keyRelease);
    }
}


