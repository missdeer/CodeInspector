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
    void wait(int milliseconds);
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
private:
    QNetworkReply* m_reply;
    QVariant m_data;
    QByteArray m_content;
};


#endif // NETWORKREPLYHELPER_H
