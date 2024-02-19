#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include "godboltagent.h"
#include "backendinterface.hpp"
#include "compiler.hpp"
#include "networkreplyhelper.h"
#include "networkrequesthelper.h"
#include "scopedguard.h"

GodboltAgent::GodboltAgent(QNetworkAccessManager &nam, QObject *parent) : QObject(parent), m_nam(nam) {}

void GodboltAgent::initialize(BackendInterface *backend, const QString &baseURL)
{
    m_backend    = backend;
    m_apiBaseURL = baseURL;
}

void GodboltAgent::compile(const CompileInfo &ci)
{
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO;
#endif
    auto compilerList = m_backend->getCompilerList(ci.language);

    QJsonObject compilerOptionsObj;
    compilerOptionsObj.insert(QStringLiteral("produceCfg"), false);
    if (m_compilerOptions & CO_AST)
    {
        compilerOptionsObj.insert(QStringLiteral("produceAst"), true);
    }
    if (m_compilerOptions & CO_OPTIMIZATION)
    {
        compilerOptionsObj.insert(QStringLiteral("produceOptInfo"), true);
    }
    if (m_compilerOptions & CO_LLVMIR)
    {
        compilerOptionsObj.insert(QStringLiteral("produceIr"), true);
    }
    if (m_compilerOptions & CO_GCCTREERTL)
    {
        QJsonObject produceGccDumpObj;
        produceGccDumpObj.insert(QStringLiteral("opened"), true);
        produceGccDumpObj.insert(QStringLiteral("pass"), m_selectedGCCDumpPass);
        produceGccDumpObj.insert(QStringLiteral("treeDump"), m_gccTreeEnabled);
        produceGccDumpObj.insert(QStringLiteral("rtlDump"), m_rtlEnabled);
        compilerOptionsObj.insert(QStringLiteral("produceGccDump"), QJsonValue::fromVariant(produceGccDumpObj));
    }

    QJsonArray toolsArray;
    if (m_compilerOptions & CO_LLVMMCA)
    {
        QJsonObject toolObj;
        toolObj.insert(QStringLiteral("id"), "llvm-mcatrunk");
        toolObj.insert(QStringLiteral("args"), m_llvmMCAOptions);
        toolsArray.append(QJsonValue::fromVariant(toolObj));
    }
    if (m_compilerOptions & CO_CLANGTIDY)
    {
        QJsonObject toolObj;
        toolObj.insert(QStringLiteral("id"), "clangtidytrunk");
        toolObj.insert(QStringLiteral("args"), m_clangTidyOptions);
        toolsArray.append(QJsonValue::fromVariant(toolObj));
    }
    if (m_compilerOptions & CO_PAHOLE)
    {
        QJsonObject toolObj;
        toolObj.insert(QStringLiteral("id"), "pahole");
        toolObj.insert(QStringLiteral("args"), m_paholeOptions);
        toolsArray.append(QJsonValue::fromVariant(toolObj));
    }
    if (m_compilerOptions & CO_READELF)
    {
        QJsonObject toolObj;
        toolObj.insert(QStringLiteral("id"), "readelf");
        toolObj.insert(QStringLiteral("args"), m_readElfOptions);
        toolsArray.append(QJsonValue::fromVariant(toolObj));
    }
    if (m_compilerOptions & CO_LDD)
    {
        QJsonObject toolObj;
        toolObj.insert(QStringLiteral("id"), "ldd");
        toolObj.insert(QStringLiteral("args"), m_lddOptions);
        toolsArray.append(QJsonValue::fromVariant(toolObj));
    }
    if (m_compilerOptions & CO_X86TO6502)
    {
        QJsonObject toolObj;
        toolObj.insert(QStringLiteral("id"), "x86to6502");
        toolObj.insert(QStringLiteral("args"), m_x86To6502Options);
        toolsArray.append(QJsonValue::fromVariant(toolObj));
    }
    if (m_compilerOptions & CO_INCLUDE_WHAT_YOU_USE)
    {
        QJsonObject toolObj;
        toolObj.insert(QStringLiteral("id"), "iwyu");
        toolObj.insert(QStringLiteral("args"), m_includeWhatYouUseOptions);
        toolsArray.append(QJsonValue::fromVariant(toolObj));
    }

    QJsonObject filtersObj;
    struct
    {
        QString     key;
        const bool &value;
    } filterMap[] = {
        {QStringLiteral("binary"), ci.binary},
        {QStringLiteral("labels"), ci.labels},
        {QStringLiteral("trim"), ci.trim},
        {QStringLiteral("directives"), ci.directives},
        {QStringLiteral("intel"), ci.intel},
        {QStringLiteral("commentOnly"), ci.commentOnly},
        {QStringLiteral("demangle"), ci.demangle},
        {QStringLiteral("libraryCode"), ci.functions},
        {QStringLiteral("execute"), false},
    };
    for (const auto &filter : filterMap)
    {
        filtersObj.insert(filter.key, filter.value);
    }

    QJsonObject optionsObj;
    optionsObj.insert(QStringLiteral("userArguments"), ci.userArguments);
    optionsObj.insert(QStringLiteral("compilerOptions"), QJsonValue::fromVariant(compilerOptionsObj));
    optionsObj.insert(QStringLiteral("filters"), QJsonValue::fromVariant(filtersObj));
    if (!toolsArray.isEmpty())
    {
        optionsObj.insert(QStringLiteral("tools"), QJsonValue::fromVariant(toolsArray));
    }

    QJsonObject rootObj;
    rootObj.insert(QStringLiteral("source"), QString(ci.source));
    rootObj.insert(QStringLiteral("compiler"), m_backend->getCompilerId(compilerList, ci.compiler));
    rootObj.insert(QStringLiteral("options"), QJsonValue::fromVariant(optionsObj));

    QString         requestUrl = m_apiBaseURL + "/api/compiler/" + m_backend->getCompilerId(compilerList, ci.compiler) + "/compile";
    QNetworkRequest request    = NetworkRequestHelper::NewRequest(requestUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader(QByteArrayLiteral("Referer"), "https://compiler-explorer.com/");
    request.setRawHeader(QByteArrayLiteral("X-Requested-With"), "XMLHttpRequest");

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
    qDebug() << Q_FUNC_INFO;
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

    QJsonValue codeVal = docObj[QStringLiteral("code")];
    if (!codeVal.isDouble())
    {
#if defined(LOGS_ENABLED)
        qDebug() << "compilation result code is expected to be an integer:" << QString(content).left(256);
#endif
        return;
    }

    QJsonValue stdoutVal = docObj[QStringLiteral("stdout")];
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
        m_compileStderr.append(soo[QStringLiteral("text")].toString() + "\n");
    }

    QJsonValue stderrVal = docObj[QStringLiteral("stderr")];
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
        m_compileStderr.append(seo[QStringLiteral("text")].toString() + "\n");
    }

    QJsonValue asmVal = docObj[QStringLiteral("asm")];
    if (!asmVal.isArray())
    {
#if defined(LOGS_ENABLED)
        qDebug() << "compilation result asm is expected to be an integer:" << QString(content).left(256);
#endif
        return;
    }
    QJsonArray asmArray = asmVal.toArray();
    for (auto asmArrElem : asmArray)
    {
        QJsonObject asmArrElemObj = asmArrElem.toObject();
        AsmItemPtr  asmItem(new AsmItem);
        asmItem->text = asmArrElemObj[QStringLiteral("text")].toString();
        if (asmArrElemObj[QStringLiteral("source")].isDouble())
        {
            asmItem->source = asmArrElemObj[QStringLiteral("source")].toInt();
        }
        else if (asmArrElemObj[QStringLiteral("source")].isObject())
        {
            QJsonObject srcObj = asmArrElemObj[QStringLiteral("source")].toObject();
            asmItem->source    = srcObj[QStringLiteral("line")].toInt();
        }
        if (asmArrElemObj[QStringLiteral("address")].isDouble())
        {
            asmItem->address = asmArrElemObj[QStringLiteral("address")].toInt();
        }
        if (asmArrElemObj[QStringLiteral("opcodes")].isArray())
        {
            QJsonArray opcodes = asmArrElemObj[QStringLiteral("opcodes")].toArray();
            for (auto opcode : opcodes)
            {
                QString op = opcode.toString();
                bool    ok = false;
                asmItem->opcodes.append(op.toInt(&ok, 16));
            }
        }
        if (asmArrElemObj[QStringLiteral("links")].isArray())
        {
            QJsonArray links = asmArrElemObj[QStringLiteral("links")].toArray();
            for (auto link : links)
            {
                QJsonObject l = link.toObject();
                AsmLinkPtr  asmLink(new AsmLink);
                asmLink->offset = l[QStringLiteral("offset")].toInt();
                asmLink->length = l[QStringLiteral("length")].toInt();
                asmLink->to     = l[QStringLiteral("to")].toInt();
                asmItem->links.push_back(asmLink);
            }
        }

        m_asmItems.push_back(asmItem);
        m_asmContent.append(asmItem->text + "\n");
    }

    QJsonValue optOutputVal = docObj[QStringLiteral("optOutput")];
    if (optOutputVal.isArray())
    {
        QJsonArray optOutputArray = optOutputVal.toArray();
        for (auto oo : optOutputArray)
        {
            if (oo.isObject())
            {
                QJsonObject         ooo = oo.toObject();
                OptimizationItemPtr oi(new OptimizationItem);
                oi->pass     = ooo[QStringLiteral("Pass")].toString();
                oi->name     = ooo[QStringLiteral("Name")].toString();
                oi->type     = ooo[QStringLiteral("optType")].toString();
                oi->function = ooo[QStringLiteral("Function")].toString();
                oi->display  = ooo[QStringLiteral("displayString")].toString();
                m_optimizationItems.push_back(oi);
            }
        }
        emit hasOptimizationOutput();
    }

    QJsonValue astOutputVal = docObj[QStringLiteral("astOutput")];
    if (astOutputVal.isString())
    {
        m_astOutput = astOutputVal.toString();
        emit hasASTOutput();
    }

    QJsonValue hasIrOutputVal = docObj[QStringLiteral("hasIrOutput")];
    if (hasIrOutputVal.isBool() && hasIrOutputVal.toBool())
    {
        QJsonValue irOutputVal = docObj[QStringLiteral("irOutput")];
        if (irOutputVal.isArray())
        {
            QJsonArray irOutputArr = irOutputVal.toArray();
            for (auto a : irOutputArr)
            {
                QJsonObject   o = a.toObject();
                LLVMIRItemPtr ir(new LLVMIRItem);
                ir->text = o[QStringLiteral("text")].toString();
                if (o[QStringLiteral("scope")].isString())
                {
                    ir->scope = o[QStringLiteral("scope")].toString();
                }
                if (o[QStringLiteral("source")].isObject())
                {
                    auto so = o[QStringLiteral("source")].toObject();
                    if (so[QStringLiteral("file")].isString())
                    {
                        ir->sourceFile = so[QStringLiteral("file")].toString();
                    }
                    if (so[QStringLiteral("line")].isDouble())
                    {
                        ir->sourceLine = so[QStringLiteral("line")].toInt();
                    }
                }
                m_llvmIRItems.push_back(ir);
                m_llvmIRContent.append(ir->text + "\n");
            }
            if (!m_llvmIRItems.empty())
            {
                emit hasLLVMIROutput();
            }
        }
    }

    QJsonValue gccDumpOutputVal = docObj[QStringLiteral("gccDumpOutput")];
    if (gccDumpOutputVal.isObject())
    {
        QJsonObject gccDumpOutputObj    = gccDumpOutputVal.toObject();
        QJsonValue  gccDumpOutputAllVal = gccDumpOutputObj[QStringLiteral("all")];
        if (gccDumpOutputAllVal.isArray())
        {
            QJsonArray gccDumpOutputAllArray = gccDumpOutputAllVal.toArray();
            for (auto arrElem : gccDumpOutputAllArray)
            {
                m_gccDumpAllPasses.append(arrElem.toString());
            }
        }
        QJsonValue currentPassOutputVal = gccDumpOutputObj[QStringLiteral("currentPassOutput")];
        if (currentPassOutputVal.isString())
        {
            m_currentGCCDumpPassOutput = currentPassOutputVal.toString();
        }
        QJsonValue selectedPassVal = gccDumpOutputObj[QStringLiteral("selectedPass")];
        if (selectedPassVal.isString())
        {
            m_selectedGCCDumpPass = selectedPassVal.toString();
        }
        emit hasGccDumpOutput();
    }

    QJsonValue toolsVal = docObj[QStringLiteral("tools")];
    if (toolsVal.isArray())
    {
        QJsonArray toolsArray = toolsVal.toArray();
        for (auto toolArrElem : toolsArray)
        {
            if (!toolArrElem.isObject())
            {
                continue;
            }
            QString     toolStdout;
            QString     toolStderr;
            QJsonObject toolObj   = toolArrElem.toObject();
            QJsonValue  stderrVal = toolObj[QStringLiteral("stderr")];
            if (stderrVal.isArray())
            {
                QJsonArray stderrArray = stderrVal.toArray();
                for (auto errElem : stderrArray)
                {
                    if (errElem.isObject())
                    {
                        QJsonObject errObj  = errElem.toObject();
                        QJsonValue  textVal = errObj[QStringLiteral("text")];
                        if (textVal.isString())
                        {
                            toolStderr.append(textVal.toString() + "\n");
                        }
                    }
                }
            }
            QJsonValue stdoutVal = toolObj[QStringLiteral("stdout")];
            if (stdoutVal.isArray())
            {
                QJsonArray stdoutArray = stdoutVal.toArray();
                for (auto outArrElem : stdoutArray)
                {
                    if (outArrElem.isObject())
                    {
                        QJsonObject outObj  = outArrElem.toObject();
                        QJsonValue  textVal = outObj[QStringLiteral("text")];
                        if (textVal.isString())
                        {
                            toolStdout.append(textVal.toString() + "\n");
                        }
                    }
                }
            }

            QString id = toolObj[QStringLiteral("id")].toString();
            if (id == QStringLiteral("clang-tidy"))
            {
                m_clangTidyStderr = toolStderr;
                m_clangTidyStdout = toolStdout;
                emit hasClangTidyOutput();
            }
            else if (id == QStringLiteral("llvm-mca"))
            {
                m_llvmMCAStdout = toolStdout;
                m_llvmMCAStderr = toolStderr;
                emit hasLLVMMCAOutput();
            }
            else if (id == QStringLiteral("pahole"))
            {
                m_paholeStderr = toolStderr;
                m_paholeStdout = toolStdout;
                emit hasPaholeOutput();
            }
            else if (id == QStringLiteral("ldd"))
            {
                m_lddStderr = toolStderr;
                m_lddStdout = toolStdout;
                emit hasLddOutput();
            }
            else if (id == QStringLiteral("x86to6502"))
            {
                m_x86To6502Stderr = toolStderr;
                m_x86To6502Stdout = toolStdout;
                emit hasX86To6502Output();
            }
            else if (id == QStringLiteral("readelf"))
            {
                m_readElfStderr = toolStderr;
                m_readElfStdout = toolStdout;
                emit hasReadElfOutput();
            }
            else if (id == QStringLiteral("iwyu"))
            {
                m_includeWhatYouUseStderr = toolStderr;
                m_includeWhatYouUseStdout = toolStdout;
                emit hasIncludeWhatYouUseOutput();
            }
        }
    }
}
