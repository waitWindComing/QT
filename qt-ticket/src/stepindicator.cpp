#include "stepindicator.h"
#include <QPainter>
#include <QtCore/qmath.h>

StepIndicator::StepIndicator(QList<QString> &steps, QString &currentStep, QWidget *parent)
    :QWidget(parent)
{
    _steps = steps;
    _currentStep = currentStep;
}

void StepIndicator::setCurrentStep(QString &currentStep)
{
    _currentStep = currentStep;
    update();
}

void StepIndicator::paintEvent ( QPaintEvent * event )
{
    const qreal gridWidth = 120.0;
    const qreal circleRadius = 7.0;
    const qreal totalWidth = _steps.count() * gridWidth;
    const qreal beginAt = (this->width() - totalWidth) / 2;
    const qreal circleY = 18.0;
    const qreal textY = 20.0;

    QPainter painter(this); 
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRect(0, 0, this->width(), this->height());
    painter.fillPath(path, QBrush(QColor(40, 40, 40)));

    if (_steps.count() == 0) return;

    int count = 0;
    for (QList<QString>::iterator it = _steps.begin(); it != _steps.end(); ++it) {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        path.addEllipse(QPointF(beginAt + gridWidth / 2 + count * gridWidth, circleY), circleRadius, circleRadius);
        if (*it == _currentStep) {
            painter.fillPath(path, QBrush(QColor(0, 151, 205)));
            painter.setPen(QColor(Qt::white));
            QRectF textRect(beginAt + count * gridWidth, textY, gridWidth, 40);
            painter.drawText(textRect, Qt::AlignCenter, *it);
        } else {
            painter.fillPath(path, QBrush(QColor(112, 112, 112)));
            painter.setPen(QColor(Qt::gray));
            QRectF textRect(beginAt + count * gridWidth, textY, gridWidth, 40);
            painter.drawText(textRect, Qt::AlignCenter, *it);
        }

        painter.setPen(QColor(93, 93, 93));
        if (count != 0) {
            QLineF line(beginAt + gridWidth / 2 + (count - 1) * gridWidth + circleRadius,
                        circleY,
                        beginAt + gridWidth / 2 + count * gridWidth - circleRadius,
                        circleY);
            painter.drawLine(line);
        }

        ++count;
    }
}

