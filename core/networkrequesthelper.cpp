#include "stdafx.h"

#include "networkrequesthelper.h"

namespace NetworkRequestHelper
{
    static const char *userAgent = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.12; rv:55.0) Gecko/20100101 Firefox/55.0";

    QNetworkRequest NewRequest(const QString &url)
    {
        return NewRequest(QUrl(url));
    }

    QNetworkRequest NewRequest(const QUrl &url)
    {
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::UserAgentHeader, QVariant(userAgent));
        request.setRawHeader("Accept", "application/json, text/javascript, */*; q=0.01");
        request.setRawHeader("Accept-Encoding", "gzip, deflate");
        request.setAttribute(QNetworkRequest::Http2AllowedAttribute, QVariant(true));
        return request;
    }

} // namespace NetworkRequestHelper
