#ifndef NETWORKREPLYHELPER_H
#define NETWORKREPLYHELPER_H

#include <QNetworkReply>
#include <QObject>

QT_BEGIN_NAMESPACE
class QTimer;
QT_END_NAMESPACE

inline const int networkRequestTimeout = 30000;

class NetworkReplyHelper : public QObject
{
    Q_OBJECT
public:
    explicit NetworkReplyHelper(QNetworkReply *reply, QObject *parent = nullptr);
    NetworkReplyHelper(const NetworkReplyHelper &) = delete;
    void operator=(const NetworkReplyHelper &) = delete;
    NetworkReplyHelper(NetworkReplyHelper &&)  = delete;
    void operator=(NetworkReplyHelper &&) = delete;
    ~NetworkReplyHelper() override;
    [[nodiscard]] QByteArray &content()
    {
        return m_content;
    }
    [[nodiscard]] QNetworkReply *reply()
    {
        return m_reply;
    }

    [[nodiscard]] QVariant       data() const;
    void                         setData(const QVariant &data);
    void                         setTimeout(int milliseconds);
    [[nodiscard]] const QString &getErrorMessage() const
    {
        return m_errMsg;
    }
signals:
    void done();
    void cancel();
    void errorMessage(QNetworkReply::NetworkError, QString);
public slots:
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void onErrorOccurred(QNetworkReply::NetworkError code);
    void onFinished();
    void onSslErrors(const QList<QSslError> &errors);
    void onUploadProgress(qint64 bytesSent, qint64 bytesTotal);
    void onReadyRead();
private slots:
    void onTimeout();

private:
    QNetworkReply *m_reply {nullptr};
    QTimer *       m_timeoutTimer {nullptr};
    QVariant       m_data;
    QByteArray     m_content;
    QString        m_errMsg;
};

#endif // NETWORKREPLYHELPER_H
