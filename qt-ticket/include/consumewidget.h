#ifndef CONSUME_WIDGET_H
#define CONSUME_WIDGET_H
#include "consume.h"
#include <QWidget>

class ConsumeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConsumeWidget(Consume *consume, QWidget *parent = 0);

private:
    Consume *_consume;
};

#endif

