#include "stdafx.h"
#include "networkreplyhelper.h"
#include "godboltagent.h"

GodboltAgent::GodboltAgent(QObject *parent)
    : QObject(parent)
{

}

GodboltAgent::~GodboltAgent()
{
    for (auto p : m_compilerMap.values())
    {
        delete p;
    }
}

LanguageList &GodboltAgent::getLanguageList()
{
    if (m_languageList.empty())
    {
        QByteArray content;
        if (loadLanguageList(content))
        {
            if (parseLanguageListFromJSON(content))
                return m_languageList;
        }

        qDebug() << "request language list";
        QString requestUrl = "https://godbolt.org/api/languages";
        QNetworkRequest request(requestUrl);
        request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.12; rv:55.0) Gecko/20100101 Firefox/55.0");
        request.setRawHeader("Accept", "application/json, text/javascript, */*; q=0.01");

        QNetworkReply* reply = m_nam.get(request);
        NetworkReplyHelper* replyHelper = new NetworkReplyHelper(reply);
        replyHelper->setTimeout(10000);
        connect(replyHelper, SIGNAL(done()), this, SLOT(onLanguageListRequestFinished()));
    }
    return m_languageList;
}

CompilerList &GodboltAgent::getCompilerList(const QString &name)
{
    auto it = m_compilerMap.find(name);
    if (m_compilerMap.end() == it)
    {
        m_compilerMap.insert(name, new CompilerList);
        it = m_compilerMap.find(name);
    }

    return *it.value();
}

void GodboltAgent::compile(const CompileInfo &ci)
{
    const CompilerList& compilerList = *m_compilerMap[ci.language];

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
    rootObj.insert("compiler", compilerList[ci.compilerIndex]->id);
    rootObj.insert("options", QJsonValue::fromVariant(optionsObj));

    QString requestUrl = "https://godbolt.org/api/compiler/" + compilerList[ci.compilerIndex]->id + "/compile";
    QNetworkRequest request(requestUrl);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.12; rv:55.0) Gecko/20100101 Firefox/55.0");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Referer", "https://godbolt.org/");
    request.setRawHeader("Accept", "application/json, text/javascript, */*; q=0.01");
    request.setRawHeader("X-Requested-With", "XMLHttpRequest");

    QJsonDocument doc;
    doc.setObject(rootObj);
    QByteArray postBody = doc.toJson();
    qDebug() << "post body: " << QString(postBody);
    QNetworkReply* reply = m_nam.post(request, postBody);

    NetworkReplyHelper* replyHelper = new NetworkReplyHelper(reply);
    replyHelper->setTimeout(10000);
    connect(replyHelper, SIGNAL(done()), this, SLOT(onCompileRequestFinished()));
}

bool GodboltAgent::canCompile(const QString& language, const QString& compiler)
{
    const CompilerList& compilerList = *m_compilerMap[language];
    auto it = std::find_if(compilerList.begin(), compilerList.end(),
                           [&compiler](CompilerPtr c){ return c->name == compiler;});
    return it != compilerList.end();
}

void GodboltAgent::switchLanguage(const QString &language)
{
    auto it = m_compilerMap.find(language);
    if (m_compilerMap.end() != it && !it.value()->isEmpty())
    {
        emit compilerListRetrieved();
        return;
    }
    m_compilerMap.insert(language, new CompilerList);

    QByteArray content;
    if (loadCompilerList(language, content))
    {
        parseCompilerListFromJSON(language, content);
    }

    QString requestUrl = "https://godbolt.org/api/compilers/" + getLanguageId(language);
    QNetworkRequest request(requestUrl);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.12; rv:55.0) Gecko/20100101 Firefox/55.0");
    request.setRawHeader("Accept", "application/json, text/javascript, */*; q=0.01");

    QNetworkReply* reply = m_nam.get(request);
    NetworkReplyHelper* replyHelper = new NetworkReplyHelper(reply);
    replyHelper->setData(language);
    replyHelper->setTimeout(10000);
    connect(replyHelper, SIGNAL(done()), this, SLOT(onCompilerListRequestFinished()));
}

void GodboltAgent::onCompilerListRequestFinished()
{
    NetworkReplyHelper* reply = qobject_cast<NetworkReplyHelper*>(sender());
    reply->deleteLater();

    QByteArray& content = reply->content();
    QString name = reply->data().toString();
    if (!content.isEmpty())
    {
        storeCompilerList(name, content);

        if (!parseCompilerListFromJSON(name, content))
            switchLanguage(name);
    }
    else
    {
        m_compileOutput = reply->getErrorMessage();
    }
}

void GodboltAgent::onCompileRequestFinished()
{
    m_compileOutput.clear();
    m_asmContent.clear();
    m_asmItems.clear();

    NetworkReplyHelper* reply = qobject_cast<NetworkReplyHelper*>(sender());
    reply->deleteLater();

    class Guard
    {
    public:
        explicit Guard(GodboltAgent* ga)
            : m_ga(ga)
        {}
        ~Guard() {
            emit m_ga->compiled();
        }
    private:
        GodboltAgent* m_ga;
    };
    Guard g(this);

    QByteArray& content = reply->content();

    if (content.isEmpty())
    {
        m_compileOutput = reply->getErrorMessage();
        return;
    }

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
        AsmItemPtr asmItem(new AsmItem);
        asmItem->text = o["text"].toString();
        if (o["source"].isDouble())
            asmItem->source = o["source"].toDouble();
        else if (o["source"].isObject())
        {
            QJsonObject srcObj = o["source"].toObject();
            asmItem->source = srcObj["line"].toDouble();
        }
        if (o["address"].isDouble())
            asmItem->address = o["address"].toDouble();
        if (o["opcodes"].isArray())
        {
            QJsonArray opcodes = o["opcodes"].toArray();
            for ( auto opcode : opcodes)
            {
                QString op = opcode.toString();
                bool ok = false;
                asmItem->opcodes.append(op.toInt(&ok, 16));
            }
        }
        if (o["links"].isArray())
        {
            QJsonArray links = o["links"].toArray();
            for (auto link : links)
            {
                QJsonObject l = link.toObject();
                AsmLinkPtr asmLink(new AsmLink);
                asmLink->offset = l["offset"].toInt();
                asmLink->length = l["length"].toInt();
                asmLink->to = l["to"].toInt();
                asmItem->links.push_back(asmLink);
            }
        }

        m_asmItems.push_back(asmItem);
        m_asmContent.append(asmItem->text + "\n");
    }
}

void GodboltAgent::onLanguageListRequestFinished()
{
    NetworkReplyHelper* reply = qobject_cast<NetworkReplyHelper*>(sender());
    reply->deleteLater();

    QByteArray& content = reply->content();
    storeLanguageList(content);
    if (parseLanguageListFromJSON(content))
        emit languageListRetrieved();
}

bool GodboltAgent::storeCompilerList(const QString& name, const QByteArray &content)
{
    QString d = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/compilerlist";
    QDir dir(d);
    if (!dir.exists())
        dir.mkpath(d);
    QString path = QString("%1/%2").arg(d).arg(name);
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

bool GodboltAgent::loadCompilerList(const QString &name, QByteArray &content)
{
    QString d = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QString path = QString("%1/compilerlist/%2").arg(d).arg(name);
    if (!QFile::exists(path))
        return false;
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly))
        return false;
    content = f.readAll();
    f.close();
    return true;
}

bool GodboltAgent::parseCompilerListFromJSON(const QString& language, const QByteArray &content)
{
    QJsonDocument doc = QJsonDocument::fromJson(content);

    if (!doc.isArray())
    {
        qDebug() << "compiler list is expected to be an array:" << QString(content);
        return false;
    }

    QJsonArray cl = doc.array();

    CompilerList *compilerList = m_compilerMap.find(language).value();
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
        CompilerPtr c(new Compiler);
        c->id = o["id"].toString();
        c->name = o["name"].toString();
        c->version = o["version"].toString();
        c->supportsBinary = o["supportsBinary"].toBool();
        c->supportsExecute = o["supportsExecute"].toBool();
        c->supportsIntel = o["supportsIntel"].toBool();
        newCompilerList.push_back(c);
        auto it = std::find_if(compilerList->begin(), compilerList->end(),
                               [&c](CompilerPtr compiler){
            return compiler->id == c->id && compiler->name == c->name;
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
                                   [&it](CompilerPtr compiler){
            return compiler->id == (*it)->id && compiler->name == (*it)->name;
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

bool GodboltAgent::storeLanguageList(const QByteArray &content)
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/languages";
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

bool GodboltAgent::loadLanguageList(QByteArray &content)
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/languages";
    if (!QFile::exists(path))
        return false;
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly))
    {
        qDebug() << "open file " << path << " for reading failed";
        return false;
    }
    content = f.readAll();
    f.close();
    return true;
}

bool GodboltAgent::parseLanguageListFromJSON(const QByteArray &content)
{
    QJsonDocument doc = QJsonDocument::fromJson(content);

    if (!doc.isArray())
    {
        qDebug() << "language list is expected to be an array:" << QString(content);
        return false;
    }

    QJsonArray ll = doc.array();
    m_languageList.clear();

    for (auto l : ll)
    {
        if (!l.isObject())
        {
            qDebug() << "language list item is expected to be an object:"  << QString(content);
            return false;
        }

        QJsonObject o = l.toObject();
        LanguagePtr lang(new Language);
        lang->id = o["id"].toString();
        lang->name = o["name"].toString();
        lang->monaco = o["monaco"].toString();
        lang->example = o["example"].toString();
        QJsonArray extensions = o["extensions"].toArray();
        for (auto ext : extensions)
        {
            if (!ext.isString())
            {
                qDebug() << "extensions item is expected to be a string:" << QString(content);
                return false;
            }
            lang->extensions.append(ext.toString());
        }
        QJsonArray alias = o["alias"].toArray();
        for (auto a : alias)
        {
            if (!a.isString())
            {
                qDebug() << "alias item is expected to be a string:" << QString(content);
                return false;
            }
            lang->alias.append(a.toString());
        }

        m_languageList.append(lang);
    }

    return !m_languageList.empty();
}

const QString &GodboltAgent::getLanguageId(const QString &name)
{
    auto it = std::find_if(m_languageList.begin(), m_languageList.end(),
                           [&name](LanguagePtr l) { return l->name == name;});
    return (*it)->id;
}

const AsmItemList &GodboltAgent::getAsmItems() const
{
    return m_asmItems;
}

const QString &GodboltAgent::getExample(const QString &language) const
{
    auto it = std::find_if(m_languageList.begin(), m_languageList.end(),
                           [&language](LanguagePtr l) { return l->name == language;});

    qDebug() << "example:" << (*it)->example;
    return (*it)->example;
}

const QString &GodboltAgent::getAsmContent() const
{
    return m_asmContent;
}

const QString &GodboltAgent::getCompileOutput() const
{
    return m_compileOutput;
}
