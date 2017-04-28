#ifndef REDUCE_EDITOR_H
#define REDUCE_EDITOR_H

#include <QWidget>
#include <QTableView>

class Ticket;
class UseRule;
class AvailableTicketModel;
class AvailableTicketDelegate;
class ReduceEditor : public QWidget
{
    Q_OBJECT

public:
    explicit ReduceEditor(const qulonglong &managerId, const QString &managerName, const int &level, const QString &token, const qulonglong &companyId, const QString &companyName, const double &latitude, const double &longitude, const QList<Ticket *> &tickets, const QList<UseRule *> &useRules, QWidget *parent = 0);
    ~ReduceEditor();

signals:
    void editingInputted(bool);

protected:
    void showEvent ( QShowEvent * event );

private:
    QTableView  *_ticketsView;

    AvailableTicketModel *_model;
    AvailableTicketDelegate *_delegate;
    const QList<Ticket *> &_tickets;
    const QList<UseRule *> &_useRules;

    const qulonglong &_managerId;
    const QString &_managerName;
    const int &_level;
    const QString &_token;
    const qulonglong &_companyId;
    const QString &_companyName;
    const double &_latitude;
    const double &_longitude;
};

#endif

