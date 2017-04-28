#ifndef DIGIT_INPUT_CONTEXT_H
#define DIGIT_INPUT_CONTEXT_H

#include <QInputContext>

class DigitInputPanel;
class DigitInputContext : public QInputContext
{
    Q_OBJECT

public:
    DigitInputContext(DigitInputPanel *, QObject *);

    QString identifierName();
    QString language();
    bool isComposing() const;
    void reset();

private slots:
    void sendCharacter(QChar c);

private:
    DigitInputPanel *_inputPanel;
};

#endif

