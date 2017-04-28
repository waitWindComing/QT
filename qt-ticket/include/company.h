#ifndef COMPANY_H
#define COMPANY_H
#include <QWidget>

class Company {
public:
    Company(qulonglong companyId, QString &companyName, double &latitude, double &longitude);

    inline qulonglong getCompanyId() const { return _companyId; }
    inline QString getName() const { return _companyName; }
    inline double getLatitude() const { return _latitude; }
    inline double getLongitude() const { return _longitude; }

    bool operator==(const Company & other) const;
    bool operator!=(const Company & other) const;

    static Company fromJson(QVariant var);

private:
    qulonglong _companyId;
    QString _companyName;
    double _latitude;
    double _longitude;
};

#endif

