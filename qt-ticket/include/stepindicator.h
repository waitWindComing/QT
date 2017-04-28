#ifndef STEP_INDICATOR_H
#define STEP_INDICATOR_H

#include <QWidget>
class StepIndicator : public QWidget
{
    Q_OBJECT

public:
    StepIndicator(QList<QString> &steps, QString &currentStep, QWidget *parent = 0);
    void setCurrentStep(QString &currentStep);

protected:
    void paintEvent ( QPaintEvent * event );

private:
    QList<QString> _steps;
    QString _currentStep;
};

#endif

