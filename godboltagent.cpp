#include "stdafx.h"
#include "networkreplyhelper.h"
#include "godboltagent.h"

GodboltAgent::GodboltAgent(QObject *parent)
    : QObject(parent)
{

}

GodboltAgent::~GodboltAgent()
{
    for (auto p : m_compilerLists.values())
    {
        delete p;
    }
}

const CompilerList &GodboltAgent::getCompilerList(int index)
{
    auto it = m_compilerLists.find(index);

    if (m_compilerLists.end() == it)
    {
        m_compilerLists.insert(index, new CompilerList);
        it = m_compilerLists.find(index);
    }

    return *it.value();
}

void GodboltAgent::switchCompiler(int index)
{
    auto it = m_compilerLists.find(index);
    if (m_compilerLists.end() == it)
        m_compilerLists.insert(index, new CompilerList);
    else
    {
        emit compilerListRetrieved();
        return;
    }

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
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.12; rv:55.0) Gecko/20100101 Firefox/55.0");
    request.setRawHeader("Accept", "application/json, text/javascript, */*; q=0.01");
    request.setRawHeader("Index", QString("%1").arg(index).toUtf8());

    QNetworkReply* reply = m_nam.get(request);
    NetworkReplyHelper* replyHelper = new NetworkReplyHelper(reply);
    connect(replyHelper, SIGNAL(done()), this, SLOT(onCompilerListRequestFinished()));
}

void GodboltAgent::onCompilerListRequestFinished()
{
    NetworkReplyHelper* reply = qobject_cast<NetworkReplyHelper*>(sender());
    reply->deleteLater();

    QByteArray& content = reply->content();
    QJsonDocument doc = QJsonDocument::fromJson(content);

    if (!doc.isArray())
    {
        qDebug() << "compiler list is expected to be an array" << QString(m_response);
        return;
    }

    QJsonArray cl = doc.array();

    QByteArray rawIndex = reply->reply()->request().rawHeader("Index");
    int index = rawIndex.toInt();
    CompilerList* compilerList = m_compilerLists.find(index).value();
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
        compilerList->push_back(c);
    }

    emit compilerListRetrieved();
}
