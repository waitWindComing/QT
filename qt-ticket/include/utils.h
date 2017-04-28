#ifndef UTILS_H
#define UTILS_H
#include <QString>

class Utils
{
public:
    static QString getHost();
    static QString generateLocation(double longitude, double latitude, const QString &name);
};

#endif // UTILS_H
