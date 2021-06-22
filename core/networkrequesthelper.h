#ifndef NETWORKREQUESTHELPER_H
#define NETWORKREQUESTHELPER_H

#include <QNetworkRequest>
#include <QUrl>

namespace NetworkRequestHelper
{
    QNetworkRequest NewRequest(const QString &url);
    QNetworkRequest NewRequest(const QUrl &url);
} // namespace NetworkRequestHelper

#endif // NETWORKREQUESTHELPER_H
