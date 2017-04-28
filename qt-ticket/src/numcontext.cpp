#include "numcontext.h"
#include "numpad.h"
#include <QPointer>
#include <QApplication>

NumContext::NumContext(NumPad *inputPanel, QObject *parent)
  :_inputPanel(inputPanel),
   QInputContext(parent)
{
    connect(_inputPanel, SIGNAL(characterGenerated(QChar)), SLOT(sendCharacter(QChar)));
}

QString NumContext::identifierName()
{
    return "NumContext";
}

void NumContext::reset()
{
}

bool NumContext::isComposing() const
{
    return false;
}

QString NumContext::language()
{
    return "en_US";
}

void NumContext::sendCharacter(QChar character)
{ 
    QPointer<QWidget> w = focusWidget();
 
    if (!w)                   
        return;               
 
    if (character == 127) {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
        QApplication::sendEvent(w, &keyPress);

        QKeyEvent keyRelease(QEvent::KeyRelease, Qt::Key_Backspace, Qt::NoModifier);
        QApplication::sendEvent(w, &keyRelease);
    } else if (character == 13) {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
        QApplication::sendEvent(w, &keyPress);

        QKeyEvent keyRelease(QEvent::KeyRelease, Qt::Key_Return, Qt::NoModifier);
        QApplication::sendEvent(w, &keyRelease);
    } else {
        QKeyEvent keyPress(QEvent::KeyPress, character.unicode(), Qt::NoModifier, QString(character));
        QApplication::sendEvent(w, &keyPress);

        QKeyEvent keyRelease(QEvent::KeyRelease, character.unicode(), Qt::NoModifier, QString(character));
        QApplication::sendEvent(w, &keyRelease);
    }
}


