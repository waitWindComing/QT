#ifndef TIPS_WIDGET_H
#define TIPS_WIDGET_H

#include <QLabel>
class TipsWidget : public QLabel
{
    Q_OBJECT

public:
    TipsWidget(QString &title, QList<QString> &tips, QWidget *parent = 0);
};

#endif

