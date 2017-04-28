#include "onlinelogin.h"
#include <QApplication>
#include <QByteArray>
#include <QFile>
#include <QDir>
#include <QLibrary>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSslCertificate>
#include <QSslSocket>
#include <qDebug>

#undef main
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QLibrary lib("qsqlite4.dll");
    lib.load();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    qDebug() << QSqlDatabase::drivers();
    db.setDatabaseName(QDir::homePath() + "/ticket.db");
    db.open();

    QSqlQuery query(QSqlDatabase::database());
    query.exec("CREATE TABLE IF NOT EXISTS User (companyId int, managerId int)");

    a.setFont(QFont("simsun", 10));

    QFile qss(":/ticket/ticket.qss");
    if(qss.open(QIODevice::ReadOnly | QIODevice::Text)) {
        a.setStyleSheet(qss.readAll());
        qss.close();
    }

    QFile crt(":/ticket/company.elable.net.crt");
    if (crt.open(QIODevice::ReadOnly)) {
        QByteArray bytes = crt.readAll();
        QSslCertificate certificate(bytes);
        QSslSocket::addDefaultCaCertificate(certificate);
        crt.close();
    }

    OnlineLogin w;
    w.show();

    return a.exec();
}
