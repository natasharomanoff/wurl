#ifndef WURLUTILS_H
#define WURLUTILS_H
#include <QString>
#include <QUuid>
#include <QtNetwork/QNetworkReply>

class WurlUtils
{
public:
    WurlUtils();
    static QString createId();
    static QString httpHeadersAsString(QNetworkReply* reply);
};

#endif // WURLUTILS_H
