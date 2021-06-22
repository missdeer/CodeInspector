#include "stdafx.h"

#include "godboltagent.h"
#include "backendinterface.hpp"
#include "compiler.hpp"
#include "networkreplyhelper.h"
#include "networkrequesthelper.h"
#include "scopedguard.h"

GodboltAgent::GodboltAgent(QNetworkAccessManager &nam, QObject *parent) : QObject(parent), m_nam(nam) {}

void GodboltAgent::initialize(BackendInterface *backend, const QString &baseURL)
{
    m_backend = backend;
    m_apiBaseURL = baseURL;
}

void GodboltAgent::compile(const CompileInfo &ci)
{
#if defined(LOGS_ENABLED)
    qDebug() << __FUNCTION__;
#endif
    auto compilerList = m_backend->getCompilerList(ci.language);

    QJsonObject compilerOptionsObj;
    compilerOptionsObj.insert("produceCfg", false);
    if (m_compilerOptions & CO_AST)
        compilerOptionsObj.insert("produceAst", true);
    if (m_compilerOptions & CO_OPTIMIZATION)
        compilerOptionsObj.insert("produceOptInfo", true);
    if (m_compilerOptions & CO_LLVMIR)
        compilerOptionsObj.insert("produceIr", true);
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
    if (m_compilerOptions & CO_READELF)
    {
        QJsonObject toolObj;
        toolObj.insert("id", "readelf");
        toolObj.insert("args", m_readElfOptions);
        toolsArray.append(QJsonValue::fromVariant(toolObj));
    }
    if (m_compilerOptions & CO_LDD)
    {
        QJsonObject toolObj;
        toolObj.insert("id", "ldd");
        toolObj.insert("args", m_lddOptions);
        toolsArray.append(QJsonValue::fromVariant(toolObj));
    }
    if (m_compilerOptions & CO_X86TO6502)
    {
        QJsonObject toolObj;
        toolObj.insert("id", "x86to6502");
        toolObj.insert("args", m_x86To6502Options);
        toolsArray.append(QJsonValue::fromVariant(toolObj));
    }
    if (m_compilerOptions & CO_INCLUDE_WHAT_YOU_USE)
    {
        QJsonObject toolObj;
        toolObj.insert("id", "iwyu");
        toolObj.insert("args", m_includeWhatYouUseOptions);
        toolsArray.append(QJsonValue::fromVariant(toolObj));
    }

    QJsonObject filtersObj;
    struct
    {
        QString     key;
        const bool &value;
    } filterMap[] = {
        {"binary", ci.binary},
        {"labels", ci.labels},
        {"trim", ci.trim},
        {"directives", ci.directives},
        {"intel", ci.intel},
        {"commentOnly", ci.commentOnly},
        {"demangle", ci.demangle},
        {"libraryCode", ci.functions},
        {"execute", false},
    };
    for (const auto &f : filterMap)
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

    QString         requestUrl = m_apiBaseURL + "/api/compiler/" + m_backend->getCompilerId(compilerList, ci.compiler) + "/compile";
    QNetworkRequest request    = NetworkRequestHelper::NewRequest(requestUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Referer", "https://godbolt.org/");
    request.setRawHeader("X-Requested-With", "XMLHttpRequest");

    QJsonDocument doc;
    doc.setObject(rootObj);
    QByteArray postBody = doc.toJson();
#if defined(LOGS_ENABLED)
    qDebug() << "post body: " << QString(postBody);
#endif
    auto *reply       = m_nam.post(request, postBody);
    auto *replyHelper = new NetworkReplyHelper(reply);
    replyHelper->setTimeout(networkRequestTimeout);
    connect(replyHelper, &NetworkReplyHelper::done, this, &GodboltAgent::onCompileRequestFinished);
}

void GodboltAgent::onCompileRequestFinished()
{
#if defined(LOGS_ENABLED)
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
    m_lddStderr.clear();
    m_lddStdout.clear();
    m_x86To6502Stderr.clear();
    m_x86To6502Stdout.clear();
    m_readElfStderr.clear();
    m_readElfStdout.clear();
    m_includeWhatYouUseStderr.clear();
    m_includeWhatYouUseStdout.clear();
    m_asmContent.clear();
    m_asmItems.clear();
    m_llvmIRContent.clear();
    m_llvmIRItems.clear();
    m_gccDumpAllPasses.clear();
    m_currentGCCDumpPassOutput.clear();
    m_selectedGCCDumpPass.clear();
    m_astOutput.clear();
    m_optimizationItems.clear();

    auto *reply = qobject_cast<NetworkReplyHelper *>(sender());
    reply->deleteLater();

    ScopedGuard emitCompiled([this]() { emit compiled(); });

    QByteArray &content = reply->content();

    if (content.isEmpty())
    {
        m_compileStderr = reply->getErrorMessage();
#if defined(LOGS_ENABLED)
        qDebug() << "compiling error:" << m_compileStderr;
#endif
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(content);

    if (!doc.isObject())
    {
#if defined(LOGS_ENABLED)
        qDebug() << "compilation result is expected to be an object:" << QString(content).left(256);
#endif
        return;
    }
#if defined(LOGS_ENABLED)
    qDebug() << doc;
#endif
    QJsonObject docObj = doc.object();

    QJsonValue codeVal = docObj["code"];
    if (!codeVal.isDouble())
    {
#if defined(LOGS_ENABLED)
        qDebug() << "compilation result code is expected to be an integer:" << QString(content).left(256);
#endif
        return;
    }

    QJsonValue stdoutVal = docObj["stdout"];
    if (!stdoutVal.isArray())
    {
#if defined(LOGS_ENABLED)
        qDebug() << "compilation result stdout is expected to be an integer:" << QString(content).left(256);
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
#if defined(LOGS_ENABLED)
        qDebug() << "compilation result stderr is expected to be an integer:" << QString(content).left(256);
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
#if defined(LOGS_ENABLED)
        qDebug() << "compilation result asm is expected to be an integer:" << QString(content).left(256);
#endif
        return;
    }
    QJsonArray asmArray = asmVal.toArray();
    for (auto a : asmArray)
    {
        QJsonObject o = a.toObject();
        AsmItemPtr  asmItem(new AsmItem);
        asmItem->text = o["text"].toString();
        if (o["source"].isDouble())
            asmItem->source = o["source"].toInt();
        else if (o["source"].isObject())
        {
            QJsonObject srcObj = o["source"].toObject();
            asmItem->source    = srcObj["line"].toInt();
        }
        if (o["address"].isDouble())
            asmItem->address = o["address"].toInt();
        if (o["opcodes"].isArray())
        {
            QJsonArray opcodes = o["opcodes"].toArray();
            for (auto opcode : opcodes)
            {
                QString op = opcode.toString();
                bool    ok = false;
                asmItem->opcodes.append(op.toInt(&ok, 16));
            }
        }
        if (o["links"].isArray())
        {
            QJsonArray links = o["links"].toArray();
            for (auto link : links)
            {
                QJsonObject l = link.toObject();
                AsmLinkPtr  asmLink(new AsmLink);
                asmLink->offset = l["offset"].toInt();
                asmLink->length = l["length"].toInt();
                asmLink->to     = l["to"].toInt();
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
                QJsonObject         ooo = oo.toObject();
                OptimizationItemPtr oi(new OptimizationItem);
                oi->pass     = ooo["Pass"].toString();
                oi->name     = ooo["Name"].toString();
                oi->type     = ooo["optType"].toString();
                oi->function = ooo["Function"].toString();
                oi->display  = ooo["displayString"].toString();
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

    QJsonValue hasIrOutputVal = docObj["hasIrOutput"];
    if (hasIrOutputVal.isBool() && hasIrOutputVal.toBool())
    {
        QJsonValue irOutputVal = docObj["irOutput"];
        if (irOutputVal.isArray())
        {
            QJsonArray irOutputArr = irOutputVal.toArray();
            for (auto a : irOutputArr)
            {
                QJsonObject   o = a.toObject();
                LLVMIRItemPtr ir(new LLVMIRItem);
                ir->text = o["text"].toString();
                if (o["scope"].isString())
                    ir->scope = o["scope"].toString();
                if (o["source"].isObject())
                {
                    auto so = o["source"].toObject();
                    if (so["file"].isString())
                        ir->sourceFile = so["file"].toString();
                    if (so["line"].isDouble())
                        ir->sourceLine = so["line"].toInt();
                }
                m_llvmIRItems.push_back(ir);
                m_llvmIRContent.append(ir->text + "\n");
            }
            if (!m_llvmIRItems.empty())
                emit hasLLVMIROutput();
        }
    }

    QJsonValue gccDumpOutputVal = docObj["gccDumpOutput"];
    if (gccDumpOutputVal.isObject())
    {
        QJsonObject gccDumpOutputObj    = gccDumpOutputVal.toObject();
        QJsonValue  gccDumpOutputAllVal = gccDumpOutputObj["all"];
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
        for (auto t : toolsArray)
        {
            if (!t.isObject())
                continue;
            QString     toolStdout;
            QString     toolStderr;
            QJsonObject toolObj   = t.toObject();
            QJsonValue  stderrVal = toolObj["stderr"];
            if (stderrVal.isArray())
            {
                QJsonArray stderrArray = stderrVal.toArray();
                for (auto e : stderrArray)
                {
                    if (e.isObject())
                    {
                        QJsonObject errObj  = e.toObject();
                        QJsonValue  textVal = errObj["text"];
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
                        QJsonObject outObj  = o.toObject();
                        QJsonValue  textVal = outObj["text"];
                        if (textVal.isString())
                        {
                            toolStdout.append(textVal.toString() + "\n");
                        }
                    }
                }
            }

            QString id = toolObj["id"].toString();
            if (id == "clang-tidy")
            {
                m_clangTidyStderr = toolStderr;
                m_clangTidyStdout = toolStdout;
                emit hasClangTidyOutput();
            }
            else if (id == "llvm-mca")
            {
                m_llvmMCAStdout = toolStdout;
                m_llvmMCAStderr = toolStderr;
                emit hasLLVMMCAOutput();
            }
            else if (id == "pahole")
            {
                m_paholeStderr = toolStderr;
                m_paholeStdout = toolStdout;
                emit hasPaholeOutput();
            }
            else if (id == "ldd")
            {
                m_lddStderr = toolStderr;
                m_lddStdout = toolStdout;
                emit hasLddOutput();
            }
            else if (id == "x86to6502")
            {
                m_x86To6502Stderr = toolStderr;
                m_x86To6502Stdout = toolStdout;
                emit hasX86To6502Output();
            }
            else if (id == "readelf")
            {
                m_readElfStderr = toolStderr;
                m_readElfStdout = toolStdout;
                emit hasReadElfOutput();
            }
            else if (id == "iwyu")
            {
                m_includeWhatYouUseStderr = toolStderr;
                m_includeWhatYouUseStdout = toolStdout;
                emit hasIncludeWhatYouUseOutput();
            }
        }
    }
}
