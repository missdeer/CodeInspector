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

void GodboltAgent::compile(const CompileInfo &ci)
{
    const CompilerList& compilerList = *m_compilerLists[ci.programmingLanguageIndex];

    QJsonObject compilerOptionsObj;
    compilerOptionsObj.insert("produceOptInfo", false);

    QJsonObject filtersObj;
    struct {
        QString key;
        const bool& value;
    }
    filterMap [] =
    {
        { "binary", ci.binary},
        { "labels", ci.labels},
        { "trim", ci.trim},
        { "directives", ci.directives},
        { "intel", ci.intel},
        { "commentOnly", ci.commentOnly},
    };
    for (const auto& f : filterMap)
    {
        if (f.value)
            filtersObj.insert(f.key, true);
    }

    QJsonObject optionsObj;
    optionsObj.insert("userArguments", ci.userArguments);
    optionsObj.insert("compilerOptions", QJsonValue::fromVariant(compilerOptionsObj));
    optionsObj.insert("filters", QJsonValue::fromVariant(filtersObj));

    QJsonObject rootObj;
    rootObj.insert("source", QString(ci.source));
    rootObj.insert("compiler", compilerList[ci.compilerIndex].id);
    rootObj.insert("options", QJsonValue::fromVariant(optionsObj));

    QString requestUrl = m_backendUrls[ci.programmingLanguageIndex] + "/api/compiler/" + compilerList[ci.compilerIndex].id + "/compile";
    QNetworkRequest request(requestUrl);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.12; rv:55.0) Gecko/20100101 Firefox/55.0");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Referer", m_backendUrls[ci.programmingLanguageIndex].toUtf8());
    request.setRawHeader("Accept", "application/json, text/javascript, */*; q=0.01");
    request.setRawHeader("X-Requested-With", "XMLHttpRequest");

    QJsonDocument doc;
    doc.setObject(rootObj);
    QByteArray postBody = doc.toJson();
    qDebug() << "post body: " << QString(postBody);
    QNetworkReply* reply = m_nam.post(request, postBody);

    NetworkReplyHelper* replyHelper = new NetworkReplyHelper(reply);
    connect(replyHelper, SIGNAL(done()), this, SLOT(onCompileRequestFinished()));
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

    QString requestUrl = m_backendUrls[index] + "/api/compilers";
    QNetworkRequest request(requestUrl);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.12; rv:55.0) Gecko/20100101 Firefox/55.0");
    request.setRawHeader("Accept", "application/json, text/javascript, */*; q=0.01");

    QNetworkReply* reply = m_nam.get(request);
    NetworkReplyHelper* replyHelper = new NetworkReplyHelper(reply);
    replyHelper->setData(index);
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
        qDebug() << "compiler list is expected to be an array";
        return;
    }

    QJsonArray cl = doc.array();

    int index = reply->data().toInt();
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

void GodboltAgent::onCompileRequestFinished()
{
    m_compileOutput.clear();
    m_asmContent.clear();
    m_asmItems.clear();

    NetworkReplyHelper* reply = qobject_cast<NetworkReplyHelper*>(sender());
    reply->deleteLater();

    QByteArray& content = reply->content();
    QJsonDocument doc = QJsonDocument::fromJson(content);

    if (!doc.isObject())
    {
        qDebug() << "compilation result is expected to be an object";
        return;
    }

    QJsonObject docObj = doc.object();

    QJsonValue codeVal = docObj["code"];
    if (!codeVal.isDouble())
    {
        qDebug() << "compilation result code is expected to be an integer";
        return;
    }

    QJsonValue stdoutVal = docObj["stdout"];
    if (!stdoutVal.isArray())
    {
        qDebug() << "compilation result stdout is expected to be an integer";
        return;
    }

    QJsonArray stdoutArray = stdoutVal.toArray();
    for (auto se : stdoutArray)
    {
        QJsonObject soo = se.toObject();
        m_compileOutput.append(soo["text"].toString() + "\n");
    }

    QJsonValue stderrVal = docObj["stderr"];
    if (!stderrVal.isArray())
    {
        qDebug() << "compilation result stderr is expected to be an integer";
        return;
    }

    QJsonArray stderrArray = stderrVal.toArray();
    for (auto se : stderrArray)
    {
        QJsonObject seo = se.toObject();
        m_compileOutput.append(seo["text"].toString() + "\n");
    }

    QJsonValue asmVal = docObj["asm"];
    if (!asmVal.isArray())
    {
        qDebug() << "compilation result asm is expected to be an integer";
        return;
    }
    QJsonArray asmArray = asmVal.toArray();
    for (auto a : asmArray)
    {
        QJsonObject o = a.toObject();
        AsmItem asmItem;
        asmItem.text = o["text"].toString();
        if (o["source"].isDouble())
            asmItem.source = o["source"].toDouble();
        if (o["address"].isDouble())
            asmItem.address = o["address"].toDouble();
        if (o["opcodes"].isArray())
        {
            QJsonArray opcodes = o["opcodes"].toArray();
            for ( auto opcode : opcodes)
            {
                QString op = opcode.toString();
                bool ok = false;
                asmItem.opcodes.append(op.toInt(&ok, 16));
            }
        }
        if (o["links"].isArray())
        {
            QJsonArray links = o["links"].toArray();
            for (auto link : links)
            {
                QJsonObject l = link.toObject();
                AsmLink asmLink;
                asmLink.offset = l["offset"].toInt();
                asmLink.length = l["length"].toInt();
                asmLink.to = l["to"].toInt();
                asmItem.links.push_back(asmLink);
            }
        }

        m_asmItems.push_back(asmItem);
        m_asmContent.append(asmItem.text + "\n");
    }

    emit compiled();
}

const QVector<AsmItem> &GodboltAgent::getAsmItems() const
{
    return m_asmItems;
}

const QString &GodboltAgent::getAsmContent() const
{
    return m_asmContent;
}

const QString &GodboltAgent::getCompileOutput() const
{
    return m_compileOutput;
}
