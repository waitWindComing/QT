#include "utils.h"
#include <qDebug>

QString Utils::getHost()
{
    //return "https://company.elable.net";
    return "http://localhost:9000";
}

QString Utils::generateLocation(double longitude, double latitude, const QString &name)
{
    return QString("http://api.map.baidu.com/staticimage?width=347&height=290&center=%1,%2&zoom=14&markers=%3,%4&markerStyles=s,A,0xff0000")
             .arg(longitude, 0, 'f', 3)
             .arg(latitude, 0, 'f', 3)
             .arg(longitude, 0, 'f', 3)
             .arg(latitude, 0, 'f', 3);
}

