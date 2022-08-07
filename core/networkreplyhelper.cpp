#include <zlib.h>

#include <QTimer>

#include "networkreplyhelper.h"

QByteArray gUncompress(const QByteArray &data)
{
    if (data.size() <= 4)
    {
        qWarning("gUncompress: Input data is truncated");
        return data;
    }

    QByteArray result;

    int              ret;
    z_stream         strm;
    static const int CHUNK_SIZE = 1024;
    char             out[CHUNK_SIZE];

    /* allocate inflate state */
    strm.zalloc   = Z_NULL;
    strm.zfree    = Z_NULL;
    strm.opaque   = Z_NULL;
    strm.avail_in = data.size();
    strm.next_in  = (Bytef *)(data.data());

    ret = inflateInit2(&strm, 15 + 32); // gzip decoding
    if (ret != Z_OK)
        return data;

    // run inflate()
    do
    {
        strm.avail_out = CHUNK_SIZE;
        strm.next_out  = (Bytef *)(out);

        ret = inflate(&strm, Z_NO_FLUSH);
        Q_ASSERT(ret != Z_STREAM_ERROR); // state not clobbered

        switch (ret)
        {
        case Z_NEED_DICT:
            ret = Z_DATA_ERROR; // and fall through
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
            (void)inflateEnd(&strm);
            return data;
        }

        result.append(out, CHUNK_SIZE - strm.avail_out);
    } while (strm.avail_out == 0);

    // clean up and return
    inflateEnd(&strm);
    return result;
}

NetworkReplyHelper::NetworkReplyHelper(QNetworkReply *reply, QObject *parent) : QObject(parent), m_reply(reply)
{
    if (m_reply)
    {
        connect(m_reply, &QNetworkReply::downloadProgress, this, &NetworkReplyHelper::onDownloadProgress);
        connect(m_reply, &QNetworkReply::errorOccurred, this, &NetworkReplyHelper::onErrorOccurred);
        connect(m_reply, &QNetworkReply::finished, this, &NetworkReplyHelper::onFinished);
        connect(m_reply, &QNetworkReply::sslErrors, this, &NetworkReplyHelper::onSslErrors);
        connect(m_reply, &QNetworkReply::uploadProgress, this, &NetworkReplyHelper::onUploadProgress);
        connect(m_reply, &QNetworkReply::readyRead, this, &NetworkReplyHelper::onReadyRead);
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
        {
            m_timeoutTimer->stop();
        }
        delete m_timeoutTimer;
        m_timeoutTimer = nullptr;
    }
}

void NetworkReplyHelper::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    Q_UNUSED(bytesReceived);
    Q_UNUSED(bytesTotal);
    if (m_timeoutTimer && m_timeoutTimer->isActive())
    {
        m_timeoutTimer->start();
    }
}

void NetworkReplyHelper::onErrorOccurred(QNetworkReply::NetworkError code)
{
    Q_UNUSED(code);
    if (m_reply)
    {
        m_errMsg.append(m_reply->errorString() + "\n");
#if defined(LOGS_ENABLED)
        qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << m_errMsg;
#endif
        emit errorMessage(code, m_errMsg);
    }
}

void NetworkReplyHelper::onFinished()
{
    if (m_timeoutTimer)
    {
        m_timeoutTimer->stop();
    }

    auto contentEncoding = m_reply->rawHeader("Content-Encoding");
#if defined(LOGS_ENABLED)
    qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << " finished: " << contentEncoding << QString(m_content).left(256) << "\n";
#endif
    if (contentEncoding == "gzip" || contentEncoding == "deflate")
    {
        m_content = gUncompress(m_content);
    }

#if defined(LOGS_ENABLED)
    qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << " finished: " << QString(m_content).left(256) << "\n";
#endif
    emit done();
}

void NetworkReplyHelper::onSslErrors(const QList<QSslError> &errors)
{
    Q_FOREACH (const QSslError &e, errors)
    {
#if defined(LOGS_ENABLED)
        qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << "ssl error:" << e.errorString();
#endif
        m_errMsg.append(e.errorString() + "\n");
    }
}

void NetworkReplyHelper::onUploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    Q_UNUSED(bytesSent);
    Q_UNUSED(bytesTotal);
    if (m_timeoutTimer && m_timeoutTimer->isActive())
    {
        m_timeoutTimer->start();
    }
}

void NetworkReplyHelper::onReadyRead()
{
    auto *reply      = qobject_cast<QNetworkReply *>(sender());
    int   statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode >= 200 && statusCode < 300)
    {
        m_content.append(reply->readAll());
    }
}

void NetworkReplyHelper::onTimeout()
{
#if defined(LOGS_ENABLED)
    qDebug() << __FILE__ << __LINE__ << __FUNCTION__ << "network request timeout";
#endif
    if (m_reply && m_reply->isRunning())
    {
        m_reply->abort();
    }
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
    {
        return;
    }
    if (!m_timeoutTimer)
    {
        m_timeoutTimer = new QTimer;
        m_timeoutTimer->setSingleShot(true);
        connect(m_timeoutTimer, &QTimer::timeout, this, &NetworkReplyHelper::onTimeout);
    }
    m_timeoutTimer->start(milliseconds);
}
