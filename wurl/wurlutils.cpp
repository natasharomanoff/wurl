#include "wurlutils.h"

WurlUtils::WurlUtils()
{
}

QString WurlUtils::createId()
{
    QString id = QUuid::createUuid().toString();
    id.remove(QChar('{'));
    id.remove(QChar('}'));
    id.remove(QChar('-'));
    return id.toUpper();
}

QString WurlUtils::httpHeadersAsString(QNetworkReply* reply) {
    QString headers;
    QList<QByteArray> baReplyHeaders = reply->rawHeaderList();
    for(int i = 0; i < baReplyHeaders.size(); i++)
    {
        QString headerFieldName(baReplyHeaders[i]);
        QString headerFieldValue(reply->rawHeader(baReplyHeaders[i]));
        headers += headerFieldName + " : " + headerFieldValue + "\n";
    }

    return headers;

}
