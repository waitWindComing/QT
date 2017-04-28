#ifndef DUE_EDITOR_H
#define DUE_EDITOR_H

#include "productselectiondlg.h"
#include <QWidget>
#include <QLineEdit>

class DueEditor : public QWidget
{
    Q_OBJECT

public:
    explicit DueEditor(const qulonglong &managerId, const QString &managerName, const int &level, const QString &token, const qulonglong &companyId, const QString &companyName, const double &latitude, const double &longitude, QWidget *parent = 0);
    ~DueEditor();
    void focus();
    QString getCustomer();
    qlonglong getDue();   // 分
    QMap<CompanyProduct, int> getSelectedCompanyProducts();
    void clear();

signals:
    void editingFinished();
    void editingUnFinished();

protected:
    void paintEvent(QPaintEvent * event);

private slots:
    void handleTextChanged(const QString &);
    void handleDueReturnPressed();
    void handleProductSelection();
    void handleDelClicked();

private:
    QLineEdit  *_customerEdit;
    QLineEdit  *_dueEdit;

    QMap<CompanyProduct, int> _selectedCompanyProducts;

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

