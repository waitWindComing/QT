#include "availableticketdelegate.h"
#include "availableticketmodel.h"
#include "company.h"
#include "userule.h"
#include <QPainter>
#include <QApplication>
#include <QMouseEvent>
#include <QDateTime>
#include <QMessageBox>
#include <qDebug>

AvailableTicketDelegate::AvailableTicketDelegate(const qulonglong &companyId, const QList<UseRule *> &useRules, QObject *parent)
    : QStyledItemDelegate(parent),
    _companyId(companyId),
    _useRules(useRules)
{
}

void AvailableTicketDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    painter->save();

    const AvailableTicketModel *tmodel = static_cast<const AvailableTicketModel *>(index.model());
    const QList<Ticket *> &tickets = tmodel->getTickets();
    Ticket *current = tickets.at(index.row());
    if (current->getCompany().getCompanyId() != _companyId) {
        painter->setPen(QColor(0, 239, 162));
    }

    Qt::CheckState state = static_cast<Qt::CheckState>(index.model()->data(index, Qt::CheckStateRole).toInt());
    if (state == Qt::Checked) {
        painter->setPen(QColor(0, 169, 162));
    }
    if (index.column() == 6) {
        QStyleOptionButton check_box_style_option;
        check_box_style_option.state |= QStyle::State_Enabled;
        if (state == Qt::Checked) {
            check_box_style_option.state |= QStyle::State_On;
        } else {
            check_box_style_option.state |= QStyle::State_Off;
        }
        check_box_style_option.rect = option.rect.adjusted(15, 0, -15, 0);
        QApplication::style()->drawControl(QStyle::CE_CheckBox, &check_box_style_option, painter);
    } else if (index.column() == 4 || index.column() == 5) {
        QDateTime dt = index.data().toDateTime();
        painter->drawText(option.rect, Qt::AlignCenter, dt.toString("yyyy-M-d\nh:m:s"));
    } else {
        painter->drawText(option.rect, Qt::AlignCenter, qvariant_cast<QString>(index.data()));
    }
    painter->restore();
}

bool AvailableTicketDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::MouseButtonRelease && index.column() == 6) {
        Qt::CheckState state = static_cast<Qt::CheckState>(index.model()->data(index, Qt::CheckStateRole).toInt());
        if (Qt::Checked == state) {
            model->setData(index, Qt::Unchecked, Qt::EditRole);
        } else {
            model->setData(index, Qt::Checked, Qt::EditRole);
        }
        AvailableTicketModel *tmodel = static_cast<AvailableTicketModel *>(model);
        const QList<Ticket *> &tickets = tmodel->getTickets();
        Ticket *current = tickets.at(index.row());
        if (current->getCheckState() == Qt::Checked) {
            if (!ticketAcceptable(current)) {
                QMessageBox msgBox;
                msgBox.setText(trUtf8("您所选择的优惠券不被接受！"));
                msgBox.exec();
                model->setData(index, Qt::Unchecked, Qt::EditRole);
            }

            for (QList<Ticket *>::const_iterator i = tickets.begin(); i != tickets.end(); ++i) {
                if (current == *i) continue;
                if ((*i)->getCheckState() == Qt::Unchecked) continue;
                if (current->getCompany() != (*i)->getCompany()) {
                    QMessageBox msgBox;
                    msgBox.setText(trUtf8("必须选择相同商家发行的优惠券！"));
                    msgBox.exec();
                    model->setData(index, Qt::Unchecked, Qt::EditRole);
                    break;
                }
                if (current->getType() == Ticket::Discount) {
                    QMessageBox msgBox;
                    msgBox.setText(trUtf8("折扣券只能单独使用！"));
                    msgBox.exec();
                    model->setData(index, Qt::Unchecked, Qt::EditRole);
                    break;
                }
                if (current->getType() == Ticket::Groupon && (*i)->getType() != Ticket::Groupon) {
                    QMessageBox msgBox;
                    msgBox.setText(trUtf8("团购券不能同折扣券或代金券同时使用！"));
                    msgBox.exec();
                    model->setData(index, Qt::Unchecked, Qt::EditRole);
                    break;
                }
                if (current->getType() == Ticket::Deduction && (*i)->getType() != Ticket::Deduction) {
                    QMessageBox msgBox;
                    msgBox.setText(trUtf8("代金券不能同折扣券或团购券同时使用！"));
                    msgBox.exec();
                    model->setData(index, Qt::Unchecked, Qt::EditRole);
                    break;
                }
            }
        }

        bool allUnchecked = true;
        for (QList<Ticket *>::const_iterator i = tickets.begin(); i != tickets.end(); ++i) {
            if ((*i)->getCheckState() == Qt::Checked) {
                allUnchecked = false;
                break;
            }
        }
        if (allUnchecked) {
            emit editingInputted(false);
        } else {
            emit editingInputted(true);
        }
    }
    return true;
}

bool AvailableTicketDelegate::ticketAcceptable(Ticket *current)
{
    // tickets issued by myself must be accepted
    if (current->getCompany().getCompanyId() == _companyId) {
        return true;
    }

    for (QList<UseRule *>::const_iterator i = _useRules.begin(); i != _useRules.end(); ++i) {
        QList<Company> issuedByCompanies = (*i)->getIssuedByCompanies();
        for (QList<Company>::iterator j = issuedByCompanies.begin(); j != issuedByCompanies.end(); ++j) {
            if (current->getCompany() == *j) return true;
        }
    }
    return false;
}

