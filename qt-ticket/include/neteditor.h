#ifndef NET_EDITOR_H
#define NET_EDITOR_H

#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include <QTableView>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class Ticket;
class UseRule;
class UseTicketDelegate;
class UseRuleTableDelegate;
class NetEditor : public QWidget
{
    Q_OBJECT

public:
    explicit NetEditor(const qulonglong &managerId, const QString &managerName, const int &level, const QString &token, const qulonglong &companyId, const QString &companyName, const double &latitude, const double &longitude, const QList<Ticket *> &selectedTickets, const QList<UseRule *> &useRules, qlonglong due, QWidget *parent = 0);
    ~NetEditor();
    qlonglong getNet();   // 分
    UseRule *getSelectedUseRule();

protected:
    void showEvent ( QShowEvent * event );

signals:
    void summaryUpdated(QString &);

private slots:
    void updateSummary();

private:
    bool isGroupon();

    QTableView *_rulesView;
    UseRuleTableDelegate *_ruleDelegate;
    QTableView *_ticketsView;
    UseTicketDelegate *_ticketDelegate;

    QList<Ticket *> _tickets;
    QList<UseRule *> _useRules;
    const qulonglong &_managerId;
    const QString &_managerName;
    const int &_level;
    const QString &_token;
    const qulonglong &_companyId;
    const QString &_companyName;
    const double &_latitude;
    const double &_longitude;
    const qlonglong _due;
    qlonglong _net;
};


#endif

