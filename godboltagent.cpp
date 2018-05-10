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

bool GodboltAgent::canCompile(int programmingLanguageIndex, int compilerIndex)
{
    if (programmingLanguageIndex >= m_compilerLists.size())
        return false;

    const CompilerList& compilerList = *m_compilerLists[programmingLanguageIndex];
    return (compilerIndex >= 0 && compilerIndex < compilerList.length());
}

void GodboltAgent::switchLanguage(int index)
{
    auto it = m_compilerLists.find(index);
    if (m_compilerLists.end() != it && !it.value()->isEmpty())
    {
        emit compilerListRetrieved();
        return;
    }
    m_compilerLists.insert(index, new CompilerList);

    QByteArray content;
    if (loadCompilerList(index, content))
    {
        parseCompilerListFromJSON(index, content);
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
    int index = reply->data().toInt();
    if (!content.isEmpty())
    {
        storeCompilerList(index, content);

        if (!parseCompilerListFromJSON(index, content))
            switchLanguage(index);
    }
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
        qDebug() << "compilation result is expected to be an object:" << QString(content);
        return;
    }

    QJsonObject docObj = doc.object();

    QJsonValue codeVal = docObj["code"];
    if (!codeVal.isDouble())
    {
        qDebug() << "compilation result code is expected to be an integer:" << QString(content);
        return;
    }

    QJsonValue stdoutVal = docObj["stdout"];
    if (!stdoutVal.isArray())
    {
        qDebug() << "compilation result stdout is expected to be an integer:" << QString(content);
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
        qDebug() << "compilation result stderr is expected to be an integer:" << QString(content);
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
        qDebug() << "compilation result asm is expected to be an integer:" << QString(content);
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
        else if (o["source"].isObject())
        {
            QJsonObject srcObj = o["source"].toObject();
            asmItem.source = srcObj["line"].toDouble();
        }
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

bool GodboltAgent::storeCompilerList(int index, const QByteArray &content)
{
    QString d = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/compilerlist";
    QDir dir(d);
    if (!dir.exists())
        dir.mkpath(d);
    QString path = QString("%1/%2").arg(d).arg(index);
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly))
    {
        qDebug() << "open file " << path << " for writting failed";
        return false;
    }
    f.write(content);
    f.close();
    return true;
}

bool GodboltAgent::loadCompilerList(int index, QByteArray &content)
{
    QString d = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QString path = QString("%1/compilerlist/%2").arg(d).arg(index);
    if (!QFile::exists(path))
        return false;
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly))
        return false;
    content = f.readAll();
    f.close();
    return true;
}

bool GodboltAgent::parseCompilerListFromJSON(int index, const QByteArray &content)
{
    QJsonDocument doc = QJsonDocument::fromJson(content);

    if (!doc.isArray())
    {
        qDebug() << "compiler list is expected to be an array:" << QString(content);
        return false;
    }

    QJsonArray cl = doc.array();

    CompilerList *compilerList = m_compilerLists.find(index).value();
    CompilerList newCompilerList;
    bool changed = false;
    for ( auto a : cl)
    {
        if (!a.isObject())
        {
            qDebug() << "compiler list item is expected to be an object:"  << QString(content);
            return false;
        }
        QJsonObject o = a.toObject();
        Compiler c;
        c.id = o["id"].toString();
        c.name = o["name"].toString();
        c.version = o["version"].toString();
        c.supportsBinary = o["supportsBinary"].toBool();
        c.supportsExecute = o["supportsExecute"].toBool();
        c.supportsIntel = o["supportsIntel"].toBool();
        newCompilerList.push_back(c);
        auto it = std::find_if(compilerList->begin(), compilerList->end(),
                               [&c](const Compiler& compiler){
            return compiler.id == c.id && compiler.name == c.name;
        });
        if (compilerList->end() == it)
        {
            changed = true;
            compilerList->push_back(c);
        }
    }

    for (auto it = compilerList->begin();it != compilerList->end();)
    {
        auto findIt = std::find_if(newCompilerList.begin(), newCompilerList.end(),
                                   [&it](const Compiler& compiler){
            return compiler.id == it->id && compiler.name == it->name;
        });
        if (findIt == newCompilerList.end())
        {
            changed = true;
            it = compilerList->erase(it);
        }
        else
            ++it;
    }

    if (changed)
        emit compilerListRetrieved();

    return true;
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
