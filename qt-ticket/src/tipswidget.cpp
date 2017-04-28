#include "tipswidget.h"

TipsWidget::TipsWidget(QString &title, QList<QString> &tips, QWidget *parent)
    :QLabel(parent)
{
    QString htmlStr;

    htmlStr = "<html><head>";
    htmlStr += "<style type='text/css'>h4{margin-left:10px;color:#bababa;} p{text-indent:-10px;font-size:12px;margin-left:20px;margin-right:20px;}</style>";
    htmlStr += "</head><body>";
    htmlStr += "<h4>";
    htmlStr += title;
    htmlStr += "</h4>";
    for (QList<QString>::iterator it = tips.begin(); it != tips.end(); ++it) {
        htmlStr += "<p>&bull;&nbsp;&nbsp;";
        htmlStr += *it;
        htmlStr += "</p>";
    }
    htmlStr += "</body></html>";

    setStyleSheet("padding-top:8px; background: #484848");
    setAlignment(Qt::AlignTop | Qt::AlignLeft);
    setWordWrap(true);
    setText(htmlStr);
}

