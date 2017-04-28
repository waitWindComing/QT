#ifndef USE_RULE_EDITOR_H
#define USE_RULE_EDITOR_H

#include "company.h"
#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class UseRule;
class QLabel;
class QLineEdit;
class QButtonGroup;
class QComboBox;
class QListView;
class QRadioButton;
class CompanyModel;
class UseRuleEditor : public QWidget
{
    Q_OBJECT

public:
    enum Mode { ReadOnly, Editable };

    explicit UseRuleEditor(const qulonglong &managerId, const QString &managerName, const int &level, const QString &token, const qulonglong &companyId, const QString &companyName, const double &latitude, const double &longitude, UseRule *rule, QWidget *parent = 0);
    ~UseRuleEditor();

    void setMode(Mode mode);
    void saveRule();   // for create or update
    void delRule();    // for cut

signals:
    void rulePosted(UseRule *);
    void rulePutted(qulonglong, UseRule *);
    void ruleDeleted(qulonglong);

protected:
    void paintEvent(QPaintEvent * event);

private slots:
    void on_typeGroup_clicked(int id);
    void on_discountTypeGroup_clicked(int id);
    void on_companyType_Changed(int index);
    void handleDeleteRuleFinished();
    void handlePutRuleFinished();
    void handlePostRuleFinished();
    void handleRequestAssociateCompaniesFinished();

private:
    void requestAssociateCompanies();
    void changeRuleType(int id);
    void changeDiscountType(int id);
    bool inputToJson(QVariantMap &);

    QLineEdit  *_nameEdit;
    QLineEdit  *_leastConsumeEdit;
    QLineEdit  *_maxDeductionEdit;
    QButtonGroup *_typeGroup;
    QRadioButton *_discountRadio;
    QRadioButton *_deductionRadio;
    QButtonGroup *_discountTypeGroup;
    QRadioButton *_oldDiscountRadio;
    QRadioButton *_newDiscountRadio;
    QLabel *_rateLabel;
    QLineEdit *_rateEdit;
    QLabel *_discountLabel;
    QLineEdit *_discountEdit;
    QLabel *_convertRateLabel;
    QLineEdit *_convertRateEdit;
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
    QList<Company> _issuedByCompanies;

    QNetworkAccessManager _qnam;
    QNetworkReply *_reply;
};

#endif

