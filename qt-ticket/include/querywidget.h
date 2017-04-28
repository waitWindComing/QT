#ifndef QUERY_WIDGET_H
#define QUERY_WIDGET_H
#include "consume.h"
#include <QWidget>
#include <QToolBar>
#include <QAction>
#include <QLabel>
#include <QProgressDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class TipsWidget;
class ConsumeWidget;
class QueryWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QueryWidget(qulonglong &managerId, QString &managerName, int &level, QString &token, qulonglong &companyId, QString &companyName, double &latitude, double &longitude, QWidget *parent = 0);
    ~QueryWidget();

private slots:
    void next();
    void previous();
    void handleGetConsumeFinished();

private:
    /*
      if currentId = 0: latest
      else: previous
    */
    void requestConsume(qulonglong currentId);

    TipsWidget *_tipsWidget;
    ConsumeWidget *_consumeWidget;
    QToolBar   *_buttonBar;
    QAction    *_nextAction;
    QAction    *_previousAction;
    QProgressDialog *_waitingDialog;

    QList<Consume *> _consumes;
    int _index;    // current index in _consumes

    const qulonglong &_managerId;
    const QString &_managerName;
    const int &_level;
    const QString &_token;
    const qulonglong &_companyId;
    const QString &_companyName;
    const double &_latitude;
    const double &_longitude;

    QNetworkAccessManager _qnam;
    QNetworkReply *_getConsumeReply;
};

#endif

