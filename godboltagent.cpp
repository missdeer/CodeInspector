#include "stdafx.h"
#include "godboltagent.h"

GodboltAgent::GodboltAgent(QObject *parent) : QObject(parent)
{

}

void GodboltAgent::switchCompiler(int index)
{
    QStringList urls = {
        "https://gcc.godbolt.org",
        "https://d.godbolt.org",
        "https://rust.godbolt.org",
        "https://go.godbolt.org",
        "https://ispc.godbolt.org",
        "https://haskell.godbolt.org",
        "https://swift.godbolt.org",
    };

    QString requestUrl = urls[index] + "/api/compilers";
    QNetworkRequest request(requestUrl);
    request.setRawHeader("Accept", "application/json");

    QNetworkReply* reply = m_nam.get(request);
    m_response.clear();
    connect(reply, SIGNAL(readyRead()), this, SLOT(onCompilerListRequestReadyRead()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(onCompilerListRequestError(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(finished()), this, SLOT(onCompilerListRequestFinished()));

    m_compilerList.clear();
}

void GodboltAgent::onCompilerListRequestFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    reply->deleteLater();

    QJsonDocument doc = QJsonDocument::fromJson(m_response);

    if (!doc.isArray())
    {
        qDebug() << "compiler list is expected to be an array" << QString(m_response);
        return;
    }

    QJsonArray cl = doc.array();

    for ( auto a : cl)
    {
        if (!a.isObject())
        {
            qDebug() << "compiler list item is expected to be an object";
            return;
        }
        QJsonObject o = a.toObject();
        Compiler c;
        c.id = o["id"].toString();
        c.name = o["name"].toString();
        m_compilerList.push_back(c);
    }

    emit compilerListRetrieved();
}

void GodboltAgent::onCompilerListRequestReadyRead()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode >= 200 && statusCode < 300)
    {
        m_response.append(reply->readAll());
    }
}

void GodboltAgent::onCompilerListRequestError(QNetworkReply::NetworkError e)
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    qDebug() << "network error:" << e << reply->errorString();
}
