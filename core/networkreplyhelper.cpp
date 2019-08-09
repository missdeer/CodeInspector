#include "stdafx.h"
#include "networkreplyhelper.h"

NetworkReplyHelper::NetworkReplyHelper(QNetworkReply *reply, QObject *parent)
    : QObject(parent)
    , m_reply(reply)
    , m_timeoutTimer(nullptr)
{
    if (m_reply)
    {
        connect(m_reply, &QNetworkReply::downloadProgress, this, &NetworkReplyHelper::downloadProgress);
        connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
        connect(m_reply, &QNetworkReply::finished, this, &NetworkReplyHelper::finished);
        connect(m_reply, &QNetworkReply::sslErrors, this, &NetworkReplyHelper::sslErrors);
        connect(m_reply, &QNetworkReply::uploadProgress, this, &NetworkReplyHelper::uploadProgress);
        connect(m_reply, &QNetworkReply::readyRead, this, &NetworkReplyHelper::readyRead);
    }
}

NetworkReplyHelper::~NetworkReplyHelper()
{
    if (m_reply)
    {
        m_reply->disconnect(this);
        m_reply->deleteLater();
        m_reply = nullptr;
    }

    if (m_timeoutTimer)
    {
        if (m_timeoutTimer->isActive())
            m_timeoutTimer->stop();
        delete m_timeoutTimer;
        m_timeoutTimer = nullptr;
    }
}

void NetworkReplyHelper::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    Q_UNUSED(bytesReceived);
    Q_UNUSED(bytesTotal);
    if (m_timeoutTimer && m_timeoutTimer->isActive())
        m_timeoutTimer->start();
}

void NetworkReplyHelper::error(QNetworkReply::NetworkError code)
{
    Q_UNUSED(code);
    if (m_reply)
    {
        m_errMsg.append(m_reply->errorString() + "\n");
#if !defined(QT_NO_DEBUG)
        qDebug() << __FUNCTION__ << m_errMsg;
#endif
        emit errorMessage(code, m_errMsg);
    }
}

void NetworkReplyHelper::finished()
{
#if !defined(QT_NO_DEBUG)
    qDebug() << this << " finished: " << QString(m_content) << "\n";
#endif
    if (m_timeoutTimer)
        m_timeoutTimer->stop();

    emit done();
}

void NetworkReplyHelper::sslErrors(const QList<QSslError> &errors)
{
    Q_FOREACH(const QSslError &e, errors)
    {
#if !defined(QT_NO_DEBUG)
        qDebug() << "ssl error:" << e.errorString();
#endif
        m_errMsg.append(e.errorString() + "\n");
    }
}

void NetworkReplyHelper::uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    Q_UNUSED(bytesSent);
    Q_UNUSED(bytesTotal);
    if (m_timeoutTimer && m_timeoutTimer->isActive())
        m_timeoutTimer->start();
}

void NetworkReplyHelper::readyRead()
{
    auto* reply = qobject_cast<QNetworkReply*>(sender());
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode >= 200 && statusCode < 300) {
        m_content.append( reply->readAll());
    }
}

void NetworkReplyHelper::timeout()
{
#if !defined(QT_NO_DEBUG)
    qDebug() << "network request timeout";
#endif
    if (m_reply && m_reply->isRunning())
        m_reply->abort();
}

QVariant NetworkReplyHelper::data() const
{
    return m_data;
}

void NetworkReplyHelper::setData(const QVariant &data)
{
    m_data = data;
}

void NetworkReplyHelper::setTimeout(int milliseconds)
{
    if (!m_reply->isRunning())
        return;
    if (!m_timeoutTimer)
    {
        m_timeoutTimer = new QTimer;
        m_timeoutTimer->setSingleShot(true);
        connect(m_timeoutTimer, &QTimer::timeout, this, &NetworkReplyHelper::timeout);
    }
    m_timeoutTimer->start(milliseconds);
}

