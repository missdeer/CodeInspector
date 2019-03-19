#include "stdafx.h"
#include "networkreplyhelper.h"
#include "compiler.hpp"
#include "backendinterface.hpp"
#include "godboltagent.h"

static const QString baseUrl = QLatin1String("https://godbolt.org");

GodboltAgent::GodboltAgent(QNetworkAccessManager &nam, QObject *parent)
    : QObject(parent)
    , m_nam(nam)
{
}

void GodboltAgent::initialize(BackendInterface *backend)
{
    m_backend = backend;
}

void GodboltAgent::compile(const CompileInfo &ci)
{
    qDebug() << __FUNCTION__;
    auto compilerList = m_backend->getCompilerList(ci.language);

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
        { "demangle", ci.demangle},
    };
    for (const auto& f : filterMap)
    {
         filtersObj.insert(f.key, f.value);
    }

    QJsonObject optionsObj;
    optionsObj.insert("userArguments", ci.userArguments);
    optionsObj.insert("compilerOptions", QJsonValue::fromVariant(compilerOptionsObj));
    optionsObj.insert("filters", QJsonValue::fromVariant(filtersObj));

    QJsonObject rootObj;
    rootObj.insert("source", QString(ci.source));
    rootObj.insert("compiler", m_backend->getCompilerId(compilerList, ci.compiler));
    rootObj.insert("options", QJsonValue::fromVariant(optionsObj));

    QString requestUrl = baseUrl + "/api/compiler/" + m_backend->getCompilerId(compilerList, ci.compiler) + "/compile";
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

    auto* replyHelper = new NetworkReplyHelper(reply);
    replyHelper->setTimeout(10000);
    connect(replyHelper, SIGNAL(done()), this, SLOT(onCompileRequestFinished()));
}

void GodboltAgent::onCompileRequestFinished()
{
    qDebug() << __FUNCTION__;
    m_compileOutput.clear();
    m_asmContent.clear();
    m_asmItems.clear();

    auto* reply = qobject_cast<NetworkReplyHelper*>(sender());
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
        Guard(const Guard&) = delete;
        void operator=(const Guard&) = delete;
        Guard(Guard&&) = delete;
        void operator=(Guard&&) = delete;
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
            asmItem->source = o["source"].toInt();
        else if (o["source"].isObject())
        {
            QJsonObject srcObj = o["source"].toObject();
            asmItem->source = srcObj["line"].toInt();
        }
        if (o["address"].isDouble())
            asmItem->address = o["address"].toInt();
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

const AsmItemList &GodboltAgent::getAsmItems() const
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
