#ifndef PRE_NET_EDITOR_H
#define PRE_NET_EDITOR_H

#include <QWidget>

class CompanyCustomer;
class PreNetEditor : public QWidget
{
    Q_OBJECT

public:
    explicit PreNetEditor(const qulonglong &managerId, const QString &managerName, const int &level, const QString &token, const qulonglong &companyId, const QString &companyName, const double &latitude, const double &longitude, qlonglong due, CompanyCustomer *companyCustomer, QWidget *parent = 0);
    ~PreNetEditor();

private:
    qlonglong _due;
    CompanyCustomer *_companyCustomer;

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

