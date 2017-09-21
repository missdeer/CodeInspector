#ifndef NETWORKREPLYHELPER_H
#define NETWORKREPLYHELPER_H

#include <QObject>
#include <QNetworkReply>

class NetworkReplyHelper : public QObject
{
    Q_OBJECT
public:
    explicit NetworkReplyHelper(QNetworkReply* reply, QObject *parent = 0);
    ~NetworkReplyHelper();
    QByteArray& content() { return m_content; }
    QNetworkReply* reply() { return m_reply; }

    QVariant data() const;
    void setData(const QVariant &data);
    void setTimeout(int milliseconds);
    const QString& getErrorMessage() const { return m_errMsg; }
signals:
    void done();
    void cancel();
    void errorMessage(QNetworkReply::NetworkError , QString);
public slots:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void error(QNetworkReply::NetworkError code);
    void finished();
    void sslErrors(const QList<QSslError> & errors);
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);
    void readyRead();
private slots:
    void timeout();
private:
    QNetworkReply* m_reply;
    QTimer *m_timeoutTimer;
    QVariant m_data;
    QByteArray m_content;
    QString m_errMsg;
};


#endif // NETWORKREPLYHELPER_H
