#include "stdafx.h"
#include "scopedguard.h"
#include "networkreplyhelper.h"
#include "compiler.hpp"
#include "backendinterface.hpp"
#include "godboltagent.h"

static const QString baseUrl = QLatin1String("https://ci.minidump.info");

GodboltAgent::GodboltAgent(QNetworkAccessManager &nam, QObject *parent)
    : QObject(parent)
    , m_nam(nam)
    , m_backend(nullptr)
    , m_compilerOptions(CO_NONE)
{
}

void GodboltAgent::initialize(BackendInterface *backend)
{
    m_backend = backend;
}

void GodboltAgent::compile(const CompileInfo &ci)
{
#if !defined(QT_NO_DEBUG)
    qDebug() << __FUNCTION__;
#endif
    auto compilerList = m_backend->getCompilerList(ci.language);

    QJsonObject compilerOptionsObj;
    compilerOptionsObj.insert("produceCfg", false);
    if (m_compilerOptions & CO_AST)
        compilerOptionsObj.insert("produceAst", true);
    if (m_compilerOptions & CO_OPTIMIZATION)
        compilerOptionsObj.insert("produceOptInfo", true);
    if (m_compilerOptions & CO_GCCTREERTL)
    {
        QJsonObject produceGccDumpObj;
        produceGccDumpObj.insert("opened", true);
        produceGccDumpObj.insert("pass", m_selectedGCCDumpPass);
        produceGccDumpObj.insert("treeDump", m_gccTreeEnabled);
        produceGccDumpObj.insert("rtlDump", m_rtlEnabled);
        compilerOptionsObj.insert("produceGccDump", QJsonValue::fromVariant(produceGccDumpObj));
    }
    
    QJsonArray toolsArray;
    if (m_compilerOptions & CO_LLVMMCA)
    {
        QJsonObject toolObj;
        toolObj.insert("id", "llvm-mcatrunk");
        toolObj.insert("args", m_llvmMCAOptions);
        toolsArray.append(QJsonValue::fromVariant(toolObj));
    }
    if (m_compilerOptions & CO_CLANGTIDY)
    {        
        QJsonObject toolObj;
        toolObj.insert("id", "clangtidytrunk");
        toolObj.insert("args", m_clangTidyOptions);
        toolsArray.append(QJsonValue::fromVariant(toolObj));
    }
    if (m_compilerOptions & CO_PAHOLE)
    {        
        QJsonObject toolObj;
        toolObj.insert("id", "pahole");
        toolObj.insert("args", m_paholeOptions);
        toolsArray.append(QJsonValue::fromVariant(toolObj));
    }
    
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
        { "libraryCode", ci.functions},
        { "execute", false},
    };
    for (const auto& f : filterMap)
    {
         filtersObj.insert(f.key, f.value);
    }

    QJsonObject optionsObj;
    optionsObj.insert("userArguments", ci.userArguments);
    optionsObj.insert("compilerOptions", QJsonValue::fromVariant(compilerOptionsObj));
    optionsObj.insert("filters", QJsonValue::fromVariant(filtersObj));
    if (!toolsArray.isEmpty())
        optionsObj.insert("tools", QJsonValue::fromVariant(toolsArray));

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
#if !defined(QT_NO_DEBUG)
    qDebug() << "post body: " << QString(postBody);
#endif
    auto* reply = m_nam.post(request, postBody);
    auto* replyHelper = new NetworkReplyHelper(reply);
    replyHelper->setTimeout(10000);
    connect(replyHelper, SIGNAL(done()), this, SLOT(onCompileRequestFinished()));
}

void GodboltAgent::onCompileRequestFinished()
{
#if !defined(QT_NO_DEBUG)
    qDebug() << __FUNCTION__;
#endif
    m_compileStderr.clear();
    m_compileStdout.clear();
    m_llvmMCAStderr.clear();
    m_llvmMCAStdout.clear();
    m_paholeStderr.clear();
    m_paholeStdout.clear();
    m_clangTidyStderr.clear();
    m_clangTidyStdout.clear();    
    m_asmContent.clear();
    m_asmItems.clear();
    m_gccDumpAllPasses.clear();
    m_currentGCCDumpPassOutput.clear();
    m_selectedGCCDumpPass.clear();
    m_astOutput.clear();
    m_optimizationItems.clear();

    auto* reply = qobject_cast<NetworkReplyHelper*>(sender());
    reply->deleteLater();

    ScopedGuard emitCompiled([this](){emit compiled();});

    QByteArray& content = reply->content();

    if (content.isEmpty())
    {
        m_compileStderr = reply->getErrorMessage();
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(content);

    if (!doc.isObject())
    {
#if !defined(QT_NO_DEBUG)
        qDebug() << "compilation result is expected to be an object:" << QString(content);
#endif
        return;
    }
#if !defined(QT_NO_DEBUG)
    qDebug() << doc;
#endif
    QJsonObject docObj = doc.object();

    QJsonValue codeVal = docObj["code"];
    if (!codeVal.isDouble())
    {
#if !defined(QT_NO_DEBUG)
        qDebug() << "compilation result code is expected to be an integer:" << QString(content);
#endif
        return;
    }

    QJsonValue stdoutVal = docObj["stdout"];
    if (!stdoutVal.isArray())
    {
#if !defined(QT_NO_DEBUG)
        qDebug() << "compilation result stdout is expected to be an integer:" << QString(content);
#endif
        return;
    }

    QJsonArray stdoutArray = stdoutVal.toArray();
    for (auto se : stdoutArray)
    {
        QJsonObject soo = se.toObject();
        m_compileStderr.append(soo["text"].toString() + "\n");
    }

    QJsonValue stderrVal = docObj["stderr"];
    if (!stderrVal.isArray())
    {
#if !defined(QT_NO_DEBUG)
        qDebug() << "compilation result stderr is expected to be an integer:" << QString(content);
#endif
        return;
    }

    QJsonArray stderrArray = stderrVal.toArray();
    for (auto se : stderrArray)
    {
        QJsonObject seo = se.toObject();
        m_compileStderr.append(seo["text"].toString() + "\n");
    }

    QJsonValue asmVal = docObj["asm"];
    if (!asmVal.isArray())
    {
#if !defined(QT_NO_DEBUG)
        qDebug() << "compilation result asm is expected to be an integer:" << QString(content);
#endif
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
    
    QJsonValue optOutputVal = docObj["optOutput"];
    if (optOutputVal.isArray())
    {
        QJsonArray optOutputArray = optOutputVal.toArray();
        for (auto oo : optOutputArray)
        {
            if (oo.isObject())
            {
                QJsonObject ooo = oo.toObject();
                OptimizationItemPtr oi(new OptimizationItem);
                oi->pass = ooo["Pass"].toString();
                oi->name = ooo["Name"].toString();
                oi->type = ooo["optType"].toString();
                oi->function = ooo["Function"].toString();
                oi->display = ooo["displayString"].toString();
                m_optimizationItems.push_back(oi);
            }
        }
        emit hasOptimizationOutput();
    }
    
    QJsonValue astOutputVal = docObj["astOutput"];
    if (astOutputVal.isString())
    {
        m_astOutput = astOutputVal.toString();
        emit hasASTOutput();
    }
    
    QJsonValue gccDumpOutputVal = docObj["gccDumpOutput"];
    if (gccDumpOutputVal.isObject())
    {
        QJsonObject gccDumpOutputObj = gccDumpOutputVal.toObject();
        QJsonValue gccDumpOutputAllVal = gccDumpOutputObj["all"];
        if (gccDumpOutputAllVal.isArray())
        {
            QJsonArray gccDumpOutputAllArray = gccDumpOutputAllVal.toArray();
            for (auto a : gccDumpOutputAllArray)
            {
                m_gccDumpAllPasses.append(a.toString());
            }
        }
        QJsonValue currentPassOutputVal = gccDumpOutputObj["currentPassOutput"];
        if (currentPassOutputVal.isString())
        {
            m_currentGCCDumpPassOutput = currentPassOutputVal.toString();
        }
        QJsonValue selectedPassVal = gccDumpOutputObj["selectedPass"];
        if (selectedPassVal.isString())
        {
            m_selectedGCCDumpPass = selectedPassVal.toString();
        }
        emit hasGccDumpOutput();
    }
    
    QJsonValue toolsVal = docObj["tools"];
    if (toolsVal.isArray())
    {
        QJsonArray toolsArray = toolsVal.toArray();
        for ( auto t : toolsArray)
        {
            if (!t.isObject())
                continue;
            QString toolStdout, toolStderr;
            QJsonObject toolObj = t.toObject();
            QJsonValue stderrVal = toolObj["stderr"];
            if (stderrVal.isArray())
            {
                QJsonArray stderrArray = stderrVal.toArray();
                for (auto e : stderrArray)
                {
                    if (e.isObject())
                    {
                        QJsonObject errObj = e.toObject();
                        QJsonValue textVal = errObj["text"];
                        if (textVal.isString())
                        {
                            toolStderr.append(textVal.toString() + "\n");
                        }
                    }
                }
            }
            QJsonValue stdoutVal = toolObj["stdout"];
            if (stdoutVal.isArray())
            {
                QJsonArray stdoutArray = stdoutVal.toArray();
                for (auto o : stdoutArray)
                {
                    if (o.isObject())
                    {
                        QJsonObject outObj = o.toObject();
                        QJsonValue textVal = outObj["text"];
                        if (textVal.isString())
                        {
                            toolStdout.append(textVal.toString() + "\n");
                        }
                    }
                }
            }
            
            QString name = toolObj["name"].toString();
            if (name == "clang-tidy")
            {
                m_clangTidyStderr = toolStderr;
                m_clangTidyStdout = toolStdout;
                emit hasClangTidyOutput();
            }
            else if (name == "llvm-mca")
            {
                m_llvmMCAStdout = toolStdout;
                m_llvmMCAStderr = toolStderr;
                emit hasLLVMMCAOutput();
            }
            else if (name == "pahole")
            {
                m_paholeStderr = toolStderr;
                m_paholeStdout = toolStdout;
                emit hasPaholeOutput();
            }
        }
    }
}

const QString &GodboltAgent::getASTOutput() const
{
    return m_astOutput;
}

const QString &GodboltAgent::getPaholeStdout() const
{
    return m_paholeStdout;
}

const QString &GodboltAgent::getPaholeStderr() const
{
    return m_paholeStderr;
}

const QString &GodboltAgent::getLLVMMCAStdout() const
{
    return m_llvmMCAStdout;
}

const QString &GodboltAgent::getLLVMMCAStderr() const
{
    return m_llvmMCAStderr;
}

const QString &GodboltAgent::getClangTidyStdout() const
{
    return m_clangTidyStdout;
}

const QString &GodboltAgent::getClangTidyStderr() const
{
    return m_clangTidyStderr;
}

const QString &GodboltAgent::getSelectedGCCDumpPass() const
{
    return m_selectedGCCDumpPass;
}

const QString &GodboltAgent::getCurrentGCCDumpPassOutput() const
{
    return m_currentGCCDumpPassOutput;
}

const QStringList &GodboltAgent::getGccDumpAllPasses() const
{
    return m_gccDumpAllPasses;
}

const AsmItemList &GodboltAgent::getAsmItems() const
{
    return m_asmItems;
}

void GodboltAgent::setEnableLLVMMCA(bool enabled)
{
    if (enabled)
        m_compilerOptions |= CO_LLVMMCA;
    else
        m_compilerOptions &= ~CO_LLVMMCA;
}

void GodboltAgent::setEnableAST(bool enabled)
{
    if (enabled)
        m_compilerOptions |= CO_AST;
    else
        m_compilerOptions &= ~CO_AST;
}

void GodboltAgent::setEnableOptimization(bool enabled)
{
    if (enabled)
        m_compilerOptions |= CO_OPTIMIZATION;
    else
        m_compilerOptions &= ~CO_OPTIMIZATION;
}

void GodboltAgent::setEnableGCCTreeRTL(bool enabled)
{
    if (enabled)
        m_compilerOptions |= CO_GCCTREERTL;
    else
        m_compilerOptions &= ~CO_GCCTREERTL;
}

void GodboltAgent::setEnablePahole(bool enabled)
{
    if (enabled)
        m_compilerOptions |= CO_PAHOLE;
    else
        m_compilerOptions &= ~CO_PAHOLE;
}

void GodboltAgent::setEnableClangTidy(bool enabled)
{
    if (enabled)
        m_compilerOptions |= CO_CLANGTIDY;
    else
        m_compilerOptions &= ~CO_CLANGTIDY;
}

void GodboltAgent::setGCCTreeRTLOptions(const QString &pass, bool gccTree, bool rtl)
{
    m_selectedGCCDumpPass = pass;
    m_gccTreeEnabled = gccTree;
    m_rtlEnabled = rtl;
}

void GodboltAgent::setLLVMMCAOptions(const QString &options)
{
    m_llvmMCAOptions = options;
}

void GodboltAgent::setPaholeOptions(const QString &options)
{
    m_paholeOptions = options;
}

void GodboltAgent::setClangTidyOptions(const QString &options)
{
    m_clangTidyOptions = options;
}

const QString &GodboltAgent::getAsmContent() const
{
    return m_asmContent;
}

const QString &GodboltAgent::getCompileStderr() const
{
    return m_compileStderr;
}

const QString &GodboltAgent::getCompileStdout() const
{
    return m_compileStdout;
}

const OptimizationItemList &GodboltAgent::getOptimizationItems() const
{
    return m_optimizationItems;
}
