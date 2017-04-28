#ifndef NUM_CONTEXT_H
#define NUM_CONTEXT_H

#include <QInputContext>

class NumPad;
class NumContext : public QInputContext
{
    Q_OBJECT

public:
    NumContext(NumPad *, QObject *);

    QString identifierName();
    QString language();
    bool isComposing() const;
    void reset();

private slots:
    void sendCharacter(QChar c);

private:
    NumPad *_inputPanel;
};

#endif

