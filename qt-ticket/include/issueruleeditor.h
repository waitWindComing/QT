#ifndef ISSUE_RULE_EDITOR_H
#define ISSUE_RULE_EDITOR_H

#include "company.h"
#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class IssueRule;
class QLabel;
class QLineEdit;
class QButtonGroup;
class QComboBox;
class QListView;
class QRadioButton;
class CompanyModel;
class IssueRuleEditor : public QWidget
{
    Q_OBJECT

public:
    enum Mode { ReadOnly, Editable };

    explicit IssueRuleEditor(const qulonglong &managerId, const QString &managerName, const int &level, const QString &token, const qulonglong &companyId, const QString &companyName, const double &latitude, const double &longitude, IssueRule *rule, QWidget *parent = 0);
    ~IssueRuleEditor();

    void setMode(Mode mode);
    void saveRule();   // for create or update
    void delRule();    // for cut
    inline qulonglong getRuleId() const { return _ruleId; }

signals:
    void rulePosted(IssueRule *);
    void rulePutted(qulonglong, IssueRule *);
    void ruleDeleted(qulonglong);

protected:
    void paintEvent(QPaintEvent * event);

private slots:
    void on_typeGroup_clicked(int id);
    void on_companyType_Changed(int index);
    void handleDeleteRuleFinished();
    void handlePutRuleFinished();
    void handlePostRuleFinished();
    void handleRequestAssociateCompaniesFinished();

private:
    void setExpiredCurrentIndex(IssueRule *);
    int getExpiredMonths();
    void requestAssociateCompanies();
    void changeIssueType(int id);
    bool inputToJson(QVariantMap &);

    QLineEdit  *_nameEdit;
    QComboBox  *_expiredCombo;
    QButtonGroup *_typeGroup;
    QRadioButton *_discountRadio;
    QRadioButton *_deductionRadio;
    QLineEdit  *_startingAmountEdit;
    QLabel *_startingDeductionLabel;
    QLineEdit *_startingDeductionEdit;
    QLabel *_stepAmountLabel;
    QLineEdit *_stepAmountEdit;
    QLabel *_stepDeductionLabel;
    QLineEdit *_stepDeductionEdit;
    QLabel *_discountLabel;
    QLineEdit *_discountEdit;
    QLabel *_discountOnceLabel;
    QButtonGroup *_discountGroup;
    QRadioButton *_onceYesRadio;
    QRadioButton *_onceNoRadio;
    QListView *_companiesList;

    CompanyModel *_model;
    QList<Company> _associatedCompanies;

    QComboBox *_companyType;

    const qulonglong &_managerId;
    const QString &_managerName;
    const int &_level;
    const QString &_token;
    const qulonglong &_companyId;
    const QString &_companyName;
    const double &_latitude;
    const double &_longitude;

    qulonglong _ruleId;
    QList<Company> _validCompanies;

    QNetworkAccessManager _qnam;
    QNetworkReply *_reply;
};

#endif

