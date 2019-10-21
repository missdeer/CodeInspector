#include "stdafx.h"
#include "networkreplyhelper.h"
#include "codeinspectorapp.h"

CodeInspectorApp *ciApp = nullptr;
static const QString baseUrl = QLatin1String("https://ci.minidump.info");

CodeInspectorApp::CodeInspectorApp(QObject *parent) 
    : QObject(parent)
    , m_backend(new GodboltAgent(m_nam, this))
{
    m_backend->initialize(this);
}

QNetworkAccessManager &CodeInspectorApp::networkAccessManager()
{
    return m_nam;
}


void CodeInspectorApp::initialize()
{
    QByteArray content;

    const QString defaultConfigurationPath = ":/resource/configurations";
    QString path = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/configurations";
    bool res = false;
    if (loadConfiguration(path, content))
        res = parseConfiguration(content);
    if (!res)
    {
        content.clear();
        loadConfiguration(defaultConfigurationPath, content);
        parseConfiguration(content);
    }
    requestConfigurations();
}

void CodeInspectorApp::requestLanguageList()
{
#if !defined(QT_NO_DEBUG)
    qDebug() << __FUNCTION__;
#endif
    QString requestUrl = baseUrl + "/api/languages";
    QNetworkRequest request(requestUrl);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.12; rv:55.0) Gecko/20100101 Firefox/55.0");
    request.setRawHeader("Accept", "application/json, text/javascript, */*; q=0.01");
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, QVariant(true));

    QNetworkReply* reply = m_nam.get(request);
    auto* replyHelper = new NetworkReplyHelper(reply);
    replyHelper->setTimeout(10000);
    connect(replyHelper, SIGNAL(done()), this, SLOT(onLanguageListRequestFinished()));
}

LanguageList &CodeInspectorApp::getLanguageList()
{
#if !defined(QT_NO_DEBUG)
    qDebug() << __FUNCTION__;
#endif
    if (m_languageList.empty())
    {
        QByteArray content;
        if (loadLanguageList(content))
        {
            if (parseLanguageListFromJSON(content))
                return m_languageList;
        }
        requestLanguageList();
    }
    return m_languageList;
}

CompilerListPtr CodeInspectorApp::getCompilerList(const QString &languageName)
{
#if !defined(QT_NO_DEBUG)
    qDebug() << __FUNCTION__ << languageName << m_compilerMap.size();
#endif
    auto it = m_compilerMap.find(languageName);
    if (m_compilerMap.end() == it)
    {
        CompilerListPtr cl(new CompilerList);
        m_compilerMap.insert(languageName, cl);
        requestCompilerList(languageName);
        return cl;
    }

    return it.value();
}

LibraryListPtr CodeInspectorApp::getLibraryList(const QString &languageName)
{
    auto languageId = getLanguageId(languageName);
    auto it = m_libs.find(languageId);
    if (m_libs.end() == it)
        return nullptr;
    return it.value();
}

CompilerPtr CodeInspectorApp::getCompiler(const QString &language, const QString &compiler)
{
    auto compilerList = m_compilerMap.find(language).value();
    auto it = std::find_if(compilerList->begin(), compilerList->end(),
                           [&compiler](CompilerPtr c) { return c->name == compiler;});
    Q_ASSERT(compilerList->end() != it);
    return *it;
}

void CodeInspectorApp::requestCompilerList(const QString &language)
{
#if !defined(QT_NO_DEBUG)
    qDebug() << __FUNCTION__;
#endif
    QString requestUrl = baseUrl + "/api/compilers/" + getLanguageId(language);
    QNetworkRequest request(requestUrl);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.12; rv:55.0) Gecko/20100101 Firefox/55.0");
    request.setRawHeader("Accept", "application/json, text/javascript, */*; q=0.01");
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, QVariant(true));

    QNetworkReply* reply = m_nam.get(request);
    auto* replyHelper = new NetworkReplyHelper(reply);
    replyHelper->setData(language);
    replyHelper->setTimeout(10000);
    connect(replyHelper, SIGNAL(done()), this, SLOT(onCompilerListRequestFinished()));
}

bool CodeInspectorApp::storeCompilerList(const QString& name, const QByteArray &content)
{
#if !defined(QT_NO_DEBUG)
    qDebug() << __FUNCTION__;
#endif
    QString d = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/compilerlist";
    QDir dir(d);
    if (!dir.exists())
    {
        if (!dir.mkpath(d))
        {
#if !defined(QT_NO_DEBUG)
            qDebug() << "creating directory failed:" << d;
#endif
            return false;
        }
    }
    QString path = QString("%1/%2").arg(d, name);
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly))
    {
#if !defined(QT_NO_DEBUG)
        qDebug() << "open file " << path << " for writting failed";
#endif
        return false;
    }
    f.write(content);
    f.close();
    return true;
}

bool CodeInspectorApp::loadCompilerList(const QString &name, QByteArray &content)
{
#if !defined(QT_NO_DEBUG)
    qDebug() << __FUNCTION__;
#endif
    QString d = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QString path = QString("%1/compilerlist/%2").arg(d, name);
    if (!QFile::exists(path))
        return false;
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly))
        return false;
    content = f.readAll();
    f.close();
    return true;
}

bool CodeInspectorApp::parseCompilerListFromJSON(const QString& language, const QByteArray &content)
{
#if !defined(QT_NO_DEBUG)
    qDebug() << __FUNCTION__;
#endif
    QJsonDocument doc = QJsonDocument::fromJson(content);

    if (!doc.isArray())
    {
#if !defined(QT_NO_DEBUG)
        qDebug() << "compiler list is expected to be an array:" << QString(content).left(256);
#endif
        return false;
    }

    QJsonArray cl = doc.array();

    CompilerListPtr compilerList = m_compilerMap.find(language).value();
    CompilerListPtr newCompilerList(new CompilerList);
    bool changed = false;
    for ( auto a : cl)
    {
        if (!a.isObject())
        {
#if !defined(QT_NO_DEBUG)
            qDebug() << "compiler list item is expected to be an object:"  << QString(content).left(256);
#endif
            return false;
        }
        QJsonObject o = a.toObject();
        CompilerPtr c(new Compiler);
        c->id = o["id"].toString();
        c->name = o["name"].toString();
        c->version = o["version"].toString();
        c->includeFlag = o["includeFlag"].toString();
        c->group = o["group"].toString();
        c->groupName = o["groupName"].toString();
        c->supportsBinary = o["supportsBinary"].toBool();
        c->supportsExecute = o["supportsExecute"].toBool();
        c->supportsIntel = o["supportsIntel"].toBool();
        c->supportsDemangle = o["supportsDemangle"].toBool();
        c->supportsOptimizationOutput = o["supportsOptOutput"].toBool();
        c->supportsAstView = o["supportsAstView"].toBool();
        c->supportsCfg = o["supportsCfg"].toBool();
        if (o["supportsGccDump"].isBool())
            c->supportsGccDump = o["supportsGccDump"].toBool();
        newCompilerList->push_back(c);
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
        auto findIt = std::find_if(newCompilerList->begin(), newCompilerList->end(),
                                   [&it](CompilerPtr compiler){
            return compiler->id == (*it)->id && compiler->name == (*it)->name;
        });
        if (findIt == newCompilerList->end())
        {
            changed = true;
            it = compilerList->erase(it);
        }
        else
            ++it;
    }

    if (changed)
        emit compilerListReady();

    return true;
}

bool CodeInspectorApp::parseCompilerListFromConfiguration(QJsonArray &array)
{
#if !defined(QT_NO_DEBUG)
    qDebug() << __FUNCTION__;
#endif
    CompilerListPtr compilerList;
    for ( auto a : array)
    {
        if (!a.isObject())
        {
#if !defined(QT_NO_DEBUG)
            qDebug() << "compiler list item is expected to be an object" ;
#endif
            return false;
        }
        QJsonObject o = a.toObject();
        QString languageId = o["lang"].toString();
        QString language = getLanguageName(languageId);
        if (m_compilerMap.find(language) != m_compilerMap.end())
            compilerList = m_compilerMap.find(language).value();
        else
        {
            compilerList = CompilerListPtr(new CompilerList);
            m_compilerMap.insert(language, compilerList);
        }
        CompilerPtr c(new Compiler);
        c->id = o["id"].toString();
        c->name = o["name"].toString();
        c->version = o["version"].toString();
        c->includeFlag = o["includeFlag"].toString();
        c->group = o["group"].toString();
        c->groupName = o["groupName"].toString();
        c->supportsBinary = o["supportsBinary"].toBool();
        c->supportsExecute = o["supportsExecute"].toBool();
        c->supportsIntel = o["supportsIntel"].toBool();
        c->supportsDemangle = o["supportsDemangle"].toBool();
        c->supportsOptimizationOutput = o["supportsOptOutput"].toBool();
        c->supportsAstView = o["supportsAstView"].toBool();
        c->supportsCfg = o["supportsCfg"].toBool();
        if (o["supportsGccDump"].isBool())
            c->supportsGccDump = o["supportsGccDump"].toBool();
        c->group = o["group"].toString();
        c->groupName = o["groupName"].toString();

        auto it = std::find_if(compilerList->begin(), compilerList->end(),
                               [&c](CompilerPtr compiler){
            return compiler->id == c->id && compiler->name == c->name;
        });
        if (compilerList->end() == it)
        {
            compilerList->push_back(c);
        }
    }

    return true;
}

bool CodeInspectorApp::storeLanguageList(const QByteArray &content)
{
#if !defined(QT_NO_DEBUG)
    qDebug() << __FUNCTION__;
#endif
    QString d = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) ;
    QDir dir(d);
    if (!dir.exists())
    {
        if (!dir.mkpath(d))
        {
#if !defined(QT_NO_DEBUG)
            qDebug() << "creating directory failed:" << d;
#endif
            return false;
        }
    }
    QString path = d + "/languages";
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly))
    {
#if !defined(QT_NO_DEBUG)
        qDebug() << "open file " << path << " for writting failed";
#endif
        return false;
    }
    f.write(content);
    f.close();
    return true;
}

bool CodeInspectorApp::loadLanguageList(QByteArray &content)
{
#if !defined(QT_NO_DEBUG)
    qDebug() << __FUNCTION__;
#endif
    QString path = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/languages";
    if (!QFile::exists(path))
        return false;
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly))
    {
#if !defined(QT_NO_DEBUG)
        qDebug() << "open file " << path << " for reading failed";
#endif
        return false;
    }
    content = f.readAll();
    f.close();
    return true;
}

bool CodeInspectorApp::parseLanguageListFromJSON(const QByteArray &content)
{
#if !defined(QT_NO_DEBUG)
    qDebug() << __FUNCTION__;
#endif
    QJsonDocument doc = QJsonDocument::fromJson(content);

    if (!doc.isArray())
    {
#if !defined(QT_NO_DEBUG)
        qDebug() << "language list is expected to be an array:" << QString(content).left(256);
#endif
        return false;
    }

    QJsonArray ll = doc.array();
    m_languageList.clear();

    for (auto l : ll)
    {
        if (!l.isObject())
        {
#if !defined(QT_NO_DEBUG)
            qDebug() << "language list item is expected to be an object:"  << QString(content).left(256);
#endif
            return false;
        }

        QJsonObject o = l.toObject();
        LanguagePtr lang(new Language);
        lang->id = o["id"].toString();
        lang->name = o["name"].toString();
        lang->monaco = o["monaco"].toString();
        lang->example = o["example"].toString();
        lang->defaultCompiler = o["defaultCompiler"].toString();
        QJsonArray extensions = o["extensions"].toArray();
        for (auto ext : extensions)
        {
            if (!ext.isString())
            {
#if !defined(QT_NO_DEBUG)
                qDebug() << "extensions item is expected to be a string:" << QString(content).left(256);
#endif
                return false;
            }
            lang->extensions.append(ext.toString());
        }
        QJsonArray alias = o["alias"].toArray();
        for (auto a : alias)
        {
            if (!a.isString())
            {
#if !defined(QT_NO_DEBUG)
                qDebug() << "alias item is expected to be a string:" << QString(content).left(256);
#endif
                return false;
            }
            lang->alias.append(a.toString());
        }

        m_languageList.append(lang);
    }

    return !m_languageList.empty();
}

bool CodeInspectorApp::parseLanguageListFromConfiguration(QJsonObject &obj)
{
#if !defined(QT_NO_DEBUG)
    qDebug() << __FUNCTION__;
#endif
    m_languageList.clear();
    for (auto v : obj)
    {
        if (!v.isObject())
        {
#if !defined(QT_NO_DEBUG)
            qDebug() << "language item is expected to be an object";
#endif
            return false;
        }
        QJsonObject o = v.toObject();
        LanguagePtr lang(new Language);
        lang->id = o["id"].toString();
        lang->name = o["name"].toString();
        lang->monaco = o["monaco"].toString();
        lang->example = o["example"].toString();
        lang->defaultCompiler = o["defaultCompiler"].toString();
        QJsonArray extensions = o["extensions"].toArray();
        for (auto ext : extensions)
        {
            if (!ext.isString())
            {
#if !defined(QT_NO_DEBUG)
                qDebug() << "extensions item is expected to be a string";
#endif
                return false;
            }
            lang->extensions.append(ext.toString());
        }
        QJsonArray alias = o["alias"].toArray();
        for (auto a : alias)
        {
            if (!a.isString())
            {
#if !defined(QT_NO_DEBUG)
                qDebug() << "alias item is expected to be a string";
#endif
                return false;
            }
            lang->alias.append(a.toString());
        }

        m_languageList.append(lang);
    }
    return !m_languageList.empty();
}

bool CodeInspectorApp::parseLibListFromConfiguration(QJsonObject &obj)
{
#if !defined(QT_NO_DEBUG)
    qDebug() << __FUNCTION__;
#endif
    m_libs.clear();
    auto languagesId = obj.keys();
    for (auto& language : languagesId)
    {
        auto libs = obj[language];
        if (!libs.isObject())
        {
#if !defined(QT_NO_DEBUG)
            qDebug() << "libs is expected to be an object";
#endif
            return false;
        }
        LibraryListPtr libraryList(new LibraryList);
        m_libs.insert(language, libraryList);
        auto libsObj = libs.toObject();
        auto libsId = libsObj.keys();
        for (auto & libId : libsId)
        {
            auto lib = libsObj[libId];
            if (!lib.isObject())
            {
#if !defined(QT_NO_DEBUG)
                qDebug() << "lib is expected to be an object";
#endif
                return false;
            }
            LibraryPtr library(new Library);
            libraryList->append(library);
            auto libObj = lib.toObject();
            library->setId( libId);
            library->setName( libObj["name"].toString());
            library->setUrl( libObj["url"].toString());
            if (!libObj["description"].isNull())
                library->setDescription(libObj["description"].toString());
            auto versions = libObj["versions"].toObject();
            auto versionsId = versions.keys();
            for (auto & versionId : versionsId)
            {
                auto version = versions[versionId];
                if (!version.isObject())
                {
#if !defined(QT_NO_DEBUG)
                    qDebug() << "version is expected to be an object";
#endif
                    return false;
                }
                auto versionObj = version.toObject();
                LibraryVersionPtr ver(new LibraryVersion);
                auto v = versionObj["version"];
                if (v.isString())
                    ver->setVersion(v.toString());
                else if (v.isDouble())
                    ver->setVersion(QString("%1").arg(v.toDouble()));
                auto path = versionObj["path"].toArray();
                for (auto p : path)
                {
                    ver->appendPath(p.toString());
                }
                library->appendVersion(ver);
            }
        }
    }
    return true;
}

bool CodeInspectorApp::parseDefaultCompilerFromConfiguration(QJsonObject &obj)
{
#if !defined(QT_NO_DEBUG)
    qDebug() << __FUNCTION__;
#endif
    auto languages = obj.keys();
    m_defaultCompiler.clear();
    for (const auto& l : languages)
    {
        m_defaultCompiler.insert(l, obj[l].toString());
    }

    return !m_defaultCompiler.empty();
}

const QString &CodeInspectorApp::getLanguageId(const QString &name)
{
    if (name.isEmpty())
        return name;
    auto it = std::find_if(m_languageList.begin(), m_languageList.end(),
                           [&name](LanguagePtr l) { return l->name == name;});
    Q_ASSERT(m_languageList.end() != it);
    return (*it)->id;
}

const QString &CodeInspectorApp::getLanguageName(const QString &id)
{
    if (id.isEmpty())
        return id;
    auto it = std::find_if(m_languageList.begin(), m_languageList.end(),
                           [&id](LanguagePtr l) { return l->id == id;});
    Q_ASSERT(m_languageList.end() != it);
    return (*it)->name;
}

const QString &CodeInspectorApp::getCompilerId(CompilerListPtr compilerList, const QString &name)
{
    if (name.isEmpty())
        return name;
    auto it = std::find_if(compilerList->begin(), compilerList->end(),
                           [&name](CompilerPtr c) { return c->name == name;});
    Q_ASSERT(compilerList->end() != it);
    return (*it)->id;
}

QStringList CodeInspectorApp::getExampleList(const QString &languageName)
{
    auto languageId = getLanguageId(languageName);
    QDir dir(":/resource/example/" + languageId);
    return dir.entryList();
}

QString CodeInspectorApp::getExampleContent(const QString &languageName, const QString &exampleName)
{
    auto languageId = getLanguageId(languageName);
    QFile f(QString(":/resource/example/%1/%2").arg(languageId, exampleName));
    if (f.open(QIODevice::ReadOnly))
    {
        QByteArray b = f.readAll();
        f.close();
        return QString(b);
    }
    return "";
}

const QString &CodeInspectorApp::getDefaultCompilerName(const QString &languageName)
{
#if !defined(QT_NO_DEBUG)
    qDebug() << __FUNCTION__ << languageName;
#endif
    static QString emptyString;
    auto languageId = getLanguageId(languageName);
    auto langIt = m_defaultCompiler.find(languageId);
    if (m_defaultCompiler.end() == langIt)
        return emptyString;
    auto compilerId = langIt.value();
    auto compilerList = getCompilerList(languageName);
    auto it =std::find_if(compilerList->begin(), compilerList->end(),
                 [&compilerId](CompilerPtr c) { return c->id == compilerId;});
    if (compilerList->end() == it)
        return emptyString;
    return (*it)->name;
}

void CodeInspectorApp::requestConfigurations()
{
#if !defined(QT_NO_DEBUG)
    qDebug() << __FUNCTION__;
#endif
    QNetworkRequest request(QUrl(baseUrl + "/configurations.json"));
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.12; rv:55.0) Gecko/20100101 Firefox/55.0");
    request.setAttribute(QNetworkRequest::HTTP2AllowedAttribute, QVariant(true));
    
    QNetworkReply* reply = m_nam.get(request);
    auto* replyHelper = new NetworkReplyHelper(reply);
    replyHelper->setTimeout(10000);
    connect(replyHelper, SIGNAL(done()), this, SLOT(onConfigurationRequestFinished()));
}

bool CodeInspectorApp::storeConfiguration(const QByteArray &content)
{
#if !defined(QT_NO_DEBUG)
    qDebug() << __FUNCTION__;
#endif
    QString d = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir dir(d);
    if (!dir.exists())
    {
        if (!dir.mkpath(d))
        {
#if !defined(QT_NO_DEBUG)
            qDebug() << "creating directory failed:" << d;
#endif
            return false;
        }
    }
    QString path = d + "/configurations";
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly))
    {
#if !defined(QT_NO_DEBUG)
        qDebug() << "open file " << path << " for writting failed";
#endif
        return false;
    }
    f.write(content);
    f.close();
    return true;
}

bool CodeInspectorApp::loadConfiguration(const QString& path, QByteArray &content)
{
#if !defined(QT_NO_DEBUG)
    qDebug() << __FUNCTION__;
#endif
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly))
    {
#if !defined(QT_NO_DEBUG)
        qDebug() << "open file " << path << " for reading failed";
#endif
        return false;
    }
#if !defined(QT_NO_DEBUG)
    qDebug() << "load configurations from" << path;
#endif
    content = f.readAll();
    f.close();
    return true;
}

bool CodeInspectorApp::parseConfiguration(const QByteArray &content)
{
#if !defined(QT_NO_DEBUG)
    qDebug() << __FUNCTION__;
#endif
    QJsonDocument doc = QJsonDocument::fromJson(content);
    if (!doc.isObject())
    {
#if !defined(QT_NO_DEBUG)
        qDebug() << "configuration is expected to be an object:" << content;
#endif
        return false;
    }

    QJsonObject obj = doc.object();
    QJsonObject languages = obj["languages"].toObject();
    bool ret = parseLanguageListFromConfiguration(languages);
    QJsonObject libs = obj["libs"].toObject();
    ret &= parseLibListFromConfiguration(libs);
    QJsonArray compilers = obj["compilers"].toArray();
    ret &= parseCompilerListFromConfiguration(compilers);
    QJsonObject defaultCompiler = obj["defaultCompiler"].toObject();
    ret &= parseDefaultCompilerFromConfiguration(defaultCompiler);
    if (ret)
    {
        emit languageListReady();
        emit compilerListReady();
        emit configurationReady();
    }
    return ret;
}

const QString &CodeInspectorApp::getExample(const QString &language) const
{
    auto it = std::find_if(m_languageList.begin(), m_languageList.end(),
                           [&language](LanguagePtr l) { return l->name == language;});
    Q_ASSERT(m_languageList.end() != it);
#if !defined(QT_NO_DEBUG)
    qDebug() << "get example:" << (*it)->example;
#endif
    return (*it)->example;
}

void CodeInspectorApp::onLanguageListRequestFinished()
{
#if !defined(QT_NO_DEBUG)
    qDebug() << __FUNCTION__;
#endif
    auto* reply = qobject_cast<NetworkReplyHelper*>(sender());
    reply->deleteLater();

    QByteArray& content = reply->content();
    storeLanguageList(content);
    if (parseLanguageListFromJSON(content))
        emit languageListReady();
}

void CodeInspectorApp::onConfigurationRequestFinished()
{
#if !defined(QT_NO_DEBUG)
    qDebug() << __FUNCTION__;
#endif
    auto* reply = qobject_cast<NetworkReplyHelper*>(sender());
    reply->deleteLater();

    QByteArray& content = reply->content();
#if !defined (QT_NO_DEBUG)
    qDebug() << __FUNCTION__ << QString(content).left(256);
#endif
    if (parseConfiguration(content))
        storeConfiguration(content);
}


void CodeInspectorApp::switchLanguage(const QString &language)
{
#if !defined(QT_NO_DEBUG)
    qDebug() << __FUNCTION__ << m_compilerMap.size();
#endif
    auto it = m_compilerMap.find(language);
    if (m_compilerMap.end() != it && !it.value()->isEmpty())
    {
        emit compilerListReady();
        return;
    }
    m_compilerMap.insert(language, CompilerListPtr(new CompilerList));

    QByteArray content;
    if (loadCompilerList(language, content))
    {
        parseCompilerListFromJSON(language, content);
    }

    requestCompilerList(language);
}

void CodeInspectorApp::onCompilerListRequestFinished()
{
#if !defined(QT_NO_DEBUG)
    qDebug() << __FUNCTION__;
#endif
    auto* reply = qobject_cast<NetworkReplyHelper*>(sender());
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
        //m_compileOutput = reply->getErrorMessage();
    }
}


bool CodeInspectorApp::canCompile(const QString& language, const QString& compiler)
{
#if !defined(QT_NO_DEBUG)
    qDebug() << __FUNCTION__ << language << compiler;
#endif
    if (m_compilerMap.find(language) == m_compilerMap.end())
        return false;
    auto compilerList = m_compilerMap.find(language).value();
    auto it = std::find_if(compilerList->begin(), compilerList->end(),
                           [&compiler](CompilerPtr c){ return c->name == compiler;});
    return it != compilerList->end();
}

