#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRandomGenerator>
#include <QStandardPaths>

#include "codeinspectorapp.h"
#include "networkreplyhelper.h"
#include "networkrequesthelper.h"
#include "settings.h"

CodeInspectorApp::CodeInspectorApp(QObject *parent) : QObject(parent)
{
    m_nam.connectToHostEncrypted("compiler-explorer.com");
}

QNetworkAccessManager &CodeInspectorApp::networkAccessManager()
{
    return m_nam;
}

void CodeInspectorApp::initialize()
{
    QByteArray    content;
    const QString defaultConfigurationPath = ":/resource/configurations.json";
    auto          path                     = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/configurations.json";
    QFileInfo     fi(defaultConfigurationPath);
    QFileInfo     fi2(path);
    if (!QFile::exists(path) || fi.lastModified() > fi2.lastModified())
    {
        QFile::copy(defaultConfigurationPath, path);
#if defined(LOGS_ENABLED)
        qDebug() << Q_FUNC_INFO << __LINE__ << "use default configuration";
#endif
    }

    if (!loadConfigurationRawContent(path, content))
    {
#if defined(LOGS_ENABLED)
        qWarning() << Q_FUNC_INFO << __LINE__ << "loading configuration failed";
#endif
    }
    if (!parseConfiguration(content))
    {
#if defined(LOGS_ENABLED)
        qWarning() << Q_FUNC_INFO << __LINE__ << "parsing configuration failed";
#endif
    }
    requestConfigurations();
}

void CodeInspectorApp::requestLanguageList()
{
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO;
#endif
    QString         requestUrl = g_settings->apiBaseURL() + "/api/languages";
    QNetworkRequest request    = NetworkRequestHelper::NewRequest(requestUrl);

    auto *reply       = m_nam.get(request);
    auto *replyHelper = new NetworkReplyHelper(reply);
    replyHelper->setTimeout(networkRequestTimeout);
    connect(replyHelper, &NetworkReplyHelper::done, this, &CodeInspectorApp::onLanguageListRequestFinished);
}

LanguageList &CodeInspectorApp::getLanguageList()
{
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO;
#endif
    if (m_languageList.empty())
    {
        QByteArray content;
        if (loadLanguageList(content))
        {
            if (parseLanguageListFromJSON(content))
            {
                return m_languageList;
            }
        }
        requestLanguageList();
    }
    return m_languageList;
}

CompilerListPtr CodeInspectorApp::getCompilerList(const QString &languageName)
{
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO << languageName << m_compilerMap.size();
#endif
    auto iter = m_compilerMap.find(languageName);
    if (m_compilerMap.end() == iter)
    {
        CompilerListPtr compilerList(new CompilerList);
        m_compilerMap.insert(languageName, compilerList);
        requestCompilerList(languageName);
        return compilerList;
    }

    return iter.value();
}

LibraryListPtr CodeInspectorApp::getLibraryList(const QString &languageName)
{
    auto languageId = getLanguageId(languageName);
    auto iter       = m_libs.find(languageId);
    if (m_libs.end() != iter)
    {
        return iter.value();
    }
    return nullptr;
}

CompilerPtr CodeInspectorApp::getCompiler(const QString &language, const QString &compiler)
{
    auto compilerList = m_compilerMap.find(language).value();
    auto iter         = std::find_if(compilerList->begin(), compilerList->end(), [&compiler](CompilerPtr c) { return c->name == compiler; });
    Q_ASSERT(compilerList->end() != iter);
    return *iter;
}

void CodeInspectorApp::requestCompilerList(const QString &language)
{
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO;
#endif
    QString         requestUrl = g_settings->apiBaseURL() + "/api/inspectors/" + getLanguageId(language);
    QNetworkRequest request    = NetworkRequestHelper::NewRequest(requestUrl);

    auto *reply       = m_nam.get(request);
    auto *replyHelper = new NetworkReplyHelper(reply);
    replyHelper->setData(language);
    replyHelper->setTimeout(networkRequestTimeout);
    connect(replyHelper, &NetworkReplyHelper::done, this, &CodeInspectorApp::onCompilerListRequestFinished);
}

bool CodeInspectorApp::storeCompilerList(const QString &name, const QByteArray &content)
{
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO;
#endif
    QString dirPath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/compilerlist";
    QDir    dir(dirPath);
    if (!dir.exists())
    {
        if (!dir.mkpath(dirPath))
        {
#if defined(LOGS_ENABLED)
            qDebug() << "creating directory failed:" << d;
#endif
            return false;
        }
    }
    const QString path = dir.absoluteFilePath(name);
    QFile         file(path);
    if (!file.open(QIODevice::WriteOnly))
    {
#if defined(LOGS_ENABLED)
        qDebug() << "open file " << path << " for writting failed";
#endif
        return false;
    }
    file.write(content);
    file.close();
    return true;
}

bool CodeInspectorApp::loadCompilerList(const QString &name, QByteArray &content)
{
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO;
#endif
    const QString dirPath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    const QString path    = QString("%1/compilerlist/%2").arg(dirPath, name);
    if (!QFile::exists(path))
    {
        return false;
    }
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }
    content = file.readAll();
    file.close();
    return true;
}

bool CodeInspectorApp::parseCompilerListFromJSON(const QString &language, const QByteArray &content)
{
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO;
#endif
    QJsonDocument doc = QJsonDocument::fromJson(content);

    if (!doc.isArray())
    {
#if defined(LOGS_ENABLED)
        qDebug() << "compiler list is expected to be an array:" << QString(content).left(256);
#endif
        return false;
    }

    QJsonArray cl = doc.array();

    CompilerListPtr compilerList = m_compilerMap.find(language).value();
    CompilerListPtr newCompilerList(new CompilerList);
    bool            changed = false;
    for (auto compilerElem : cl)
    {
        if (!compilerElem.isObject())
        {
#if defined(LOGS_ENABLED)
            qDebug() << "compiler list item is expected to be an object:" << QString(content).left(256);
#endif
            return false;
        }
        QJsonObject compilerObj = compilerElem.toObject();
        CompilerPtr compiler(new Compiler);
        compiler->id                         = compilerObj[QStringLiteral("id")].toString();
        compiler->name                       = compilerObj[QStringLiteral("name")].toString();
        compiler->version                    = compilerObj[QStringLiteral("version")].toString();
        compiler->includeFlag                = compilerObj[QStringLiteral("includeFlag")].toString();
        compiler->group                      = compilerObj[QStringLiteral("group")].toString();
        compiler->groupName                  = compilerObj[QStringLiteral("groupName")].toString();
        compiler->supportsBinary             = compilerObj[QStringLiteral("supportsBinary")].toBool();
        compiler->supportsExecute            = compilerObj[QStringLiteral("supportsExecute")].toBool();
        compiler->supportsIntel              = compilerObj[QStringLiteral("supportsIntel")].toBool();
        compiler->supportsDemangle           = compilerObj[QStringLiteral("supportsDemangle")].toBool();
        compiler->supportsOptimizationOutput = compilerObj[QStringLiteral("supportsOptOutput")].toBool();
        compiler->supportsAstView            = compilerObj[QStringLiteral("supportsAstView")].toBool();
        compiler->supportsIrView             = compilerObj[QStringLiteral("supportsIrView")].toBool();
        compiler->supportsCfg                = compilerObj[QStringLiteral("supportsCfg")].toBool();
        if (compilerObj[QStringLiteral("supportsGccDump")].isBool())
        {
            compiler->supportsGccDump = compilerObj[QStringLiteral("supportsGccDump")].toBool();
        }
        newCompilerList->push_back(compiler);
        auto iter = std::find_if(compilerList->begin(), compilerList->end(), [&compiler](const CompilerPtr &compilerElem) {
            return compilerElem->id == compiler->id && compilerElem->name == compiler->name;
        });
        if (compilerList->end() == iter)
        {
            changed = true;
            compilerList->push_back(compiler);
        }
    }

    for (auto iter = compilerList->begin(); iter != compilerList->end();)
    {
        auto findIt = std::find_if(newCompilerList->begin(), newCompilerList->end(), [&iter](const CompilerPtr &compiler) {
            return compiler->id == (*iter)->id && compiler->name == (*iter)->name;
        });
        if (findIt == newCompilerList->end())
        {
            changed = true;
            iter    = compilerList->erase(iter);
        }
        else
        {
            ++iter;
        }
    }

    if (changed)
    {
        emit compilerListReady();
    }

    return true;
}

bool CodeInspectorApp::parseCompilerListFromConfiguration(QJsonArray &array)
{
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO;
#endif
    CompilerListPtr compilerList;
    for (auto arrElem : array)
    {
        if (!arrElem.isObject())
        {
#if defined(LOGS_ENABLED)
            qDebug() << "compiler list item is expected to be an object";
#endif
            return false;
        }
        QJsonObject arrObj     = arrElem.toObject();
        QString     languageId = arrObj[QStringLiteral("lang")].toString();
        QString     language   = getLanguageName(languageId);
        if (m_compilerMap.find(language) != m_compilerMap.end())
        {
            compilerList = m_compilerMap.find(language).value();
        }
        else
        {
            compilerList = CompilerListPtr(new CompilerList);
            m_compilerMap.insert(language, compilerList);
        }
        CompilerPtr compiler(new Compiler);
        compiler->id                         = arrObj[QStringLiteral("id")].toString();
        compiler->name                       = arrObj[QStringLiteral("name")].toString();
        compiler->version                    = arrObj[QStringLiteral("version")].toString();
        compiler->includeFlag                = arrObj[QStringLiteral("includeFlag")].toString();
        compiler->group                      = arrObj[QStringLiteral("group")].toString();
        compiler->groupName                  = arrObj[QStringLiteral("groupName")].toString();
        compiler->supportsBinary             = arrObj[QStringLiteral("supportsBinary")].toBool();
        compiler->supportsExecute            = arrObj[QStringLiteral("supportsExecute")].toBool();
        compiler->supportsIntel              = arrObj[QStringLiteral("supportsIntel")].toBool();
        compiler->supportsDemangle           = arrObj[QStringLiteral("supportsDemangle")].toBool();
        compiler->supportsOptimizationOutput = arrObj[QStringLiteral("supportsOptOutput")].toBool();
        compiler->supportsAstView            = arrObj[QStringLiteral("supportsAstView")].toBool();
        compiler->supportsIrView             = arrObj[QStringLiteral("supportsIrView")].toBool();
        compiler->supportsCfg                = arrObj[QStringLiteral("supportsCfg")].toBool();
        if (arrObj[QStringLiteral("supportsGccDump")].isBool())
        {
            compiler->supportsGccDump = arrObj[QStringLiteral("supportsGccDump")].toBool();
        }
        compiler->group     = arrObj[QStringLiteral("group")].toString();
        compiler->groupName = arrObj[QStringLiteral("groupName")].toString();

        auto iter = std::find_if(compilerList->begin(), compilerList->end(), [&compiler](const CompilerPtr &compilerElem) {
            return compilerElem->id == compiler->id && compilerElem->name == compiler->name;
        });
        if (compilerList->end() == iter)
        {
            compilerList->push_back(compiler);
        }
    }

    return true;
}

bool CodeInspectorApp::storeLanguageList(const QByteArray &content)
{
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO;
#endif
    const QString dirPath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir          dir(dirPath);
    if (!dir.exists())
    {
        if (!dir.mkpath(dirPath))
        {
#if defined(LOGS_ENABLED)
            qDebug() << "creating directory failed:" << d;
#endif
            return false;
        }
    }
    const QString path = dirPath + "/languages";
    QFile         file(path);
    if (!file.open(QIODevice::WriteOnly))
    {
#if defined(LOGS_ENABLED)
        qDebug() << "open file " << path << " for writting failed";
#endif
        return false;
    }
    file.write(content);
    file.close();
    return true;
}

bool CodeInspectorApp::loadLanguageList(QByteArray &content)
{
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO;
#endif
    const QString path = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/languages";
    if (!QFile::exists(path))
    {
        return false;
    }
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
#if defined(LOGS_ENABLED)
        qDebug() << "open file " << path << " for reading failed";
#endif
        return false;
    }
    content = file.readAll();
    file.close();
    return true;
}

bool CodeInspectorApp::parseLanguageListFromJSON(const QByteArray &content)
{
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO;
#endif
    QJsonDocument doc = QJsonDocument::fromJson(content);

    if (!doc.isArray())
    {
#if defined(LOGS_ENABLED)
        qDebug() << "language list is expected to be an array:" << QString(content).left(256);
#endif
        return false;
    }

    QJsonArray langArr = doc.array();
    m_languageList.clear();

    for (auto langElem : langArr)
    {
        if (!langElem.isObject())
        {
#if defined(LOGS_ENABLED)
            qDebug() << "language list item is expected to be an object:" << QString(content).left(256);
#endif
            return false;
        }

        QJsonObject langObj = langElem.toObject();
        LanguagePtr lang(new Language);
        lang->id              = langObj[QStringLiteral("id")].toString();
        lang->name            = langObj[QStringLiteral("name")].toString();
        lang->monaco          = langObj[QStringLiteral("monaco")].toString();
        lang->example         = langObj[QStringLiteral("example")].toString();
        lang->defaultCompiler = langObj[QStringLiteral("defaultCompiler")].toString();
        QJsonArray extensions = langObj[QStringLiteral("extensions")].toArray();
        for (auto ext : extensions)
        {
            if (!ext.isString())
            {
#if defined(LOGS_ENABLED)
                qDebug() << "extensions item is expected to be a string:" << QString(content).left(256);
#endif
                return false;
            }
            lang->extensions.append(ext.toString());
        }
        QJsonArray alias = langObj[QStringLiteral("alias")].toArray();
        for (auto aliasElem : alias)
        {
            if (!aliasElem.isString())
            {
#if defined(LOGS_ENABLED)
                qDebug() << "alias item is expected to be a string:" << QString(content).left(256);
#endif
                return false;
            }
            lang->alias.append(aliasElem.toString());
        }

        m_languageList.append(lang);
    }

    return !m_languageList.empty();
}

bool CodeInspectorApp::parseLanguageListFromConfiguration(QJsonObject &obj)
{
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO;
#endif
    m_languageList.clear();
    for (auto langElem : obj)
    {
        if (!langElem.isObject())
        {
#if defined(LOGS_ENABLED)
            qDebug() << "language item is expected to be an object";
#endif
            return false;
        }
        QJsonObject langObj = langElem.toObject();
        LanguagePtr lang(new Language);
        lang->id              = langObj[QStringLiteral("id")].toString();
        lang->name            = langObj[QStringLiteral("name")].toString();
        lang->monaco          = langObj[QStringLiteral("monaco")].toString();
        lang->example         = langObj[QStringLiteral("example")].toString();
        lang->defaultCompiler = langObj[QStringLiteral("defaultCompiler")].toString();
        QJsonArray extensions = langObj[QStringLiteral("extensions")].toArray();
        for (auto ext : extensions)
        {
            if (!ext.isString())
            {
#if defined(LOGS_ENABLED)
                qDebug() << "extensions item is expected to be a string";
#endif
                return false;
            }
            lang->extensions.append(ext.toString());
        }
        QJsonArray alias = langObj[QStringLiteral("alias")].toArray();
        for (auto aliasElem : alias)
        {
            if (!aliasElem.isString())
            {
#if defined(LOGS_ENABLED)
                qDebug() << "alias item is expected to be a string";
#endif
                return false;
            }
            lang->alias.append(aliasElem.toString());
        }

        m_languageList.append(lang);
    }
    return !m_languageList.empty();
}

bool CodeInspectorApp::parseLibListFromConfiguration(QJsonObject &obj)
{
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO;
#endif
    m_libs.clear();
    auto languagesId = obj.keys();
    for (auto &language : languagesId)
    {
        auto libs = obj[language];
        if (!libs.isObject())
        {
#if defined(LOGS_ENABLED)
            qDebug() << "libs is expected to be an object";
#endif
            return false;
        }
        LibraryListPtr libraryList(new LibraryList);
        m_libs.insert(language, libraryList);
        auto libsObj = libs.toObject();
        auto libsId  = libsObj.keys();
        for (auto &libId : libsId)
        {
            auto lib = libsObj[libId];
            if (!lib.isObject())
            {
#if defined(LOGS_ENABLED)
                qDebug() << "lib is expected to be an object";
#endif
                return false;
            }
            LibraryPtr library(new Library);
            libraryList->append(library);
            auto libObj = lib.toObject();
            library->setId(libId);
            library->setName(libObj[QStringLiteral("name")].toString());
            library->setUrl(libObj[QStringLiteral("url")].toString());
            if (!libObj[QStringLiteral("description")].isNull())
            {
                library->setDescription(libObj[QStringLiteral("description")].toString());
            }
            auto versions   = libObj[QStringLiteral("versions")].toObject();
            auto versionsId = versions.keys();
            for (auto &versionId : versionsId)
            {
                auto version = versions[versionId];
                if (!version.isObject())
                {
#if defined(LOGS_ENABLED)
                    qDebug() << "version is expected to be an object";
#endif
                    return false;
                }
                auto              versionObj = version.toObject();
                LibraryVersionPtr ver(new LibraryVersion);
                auto              v = versionObj[QStringLiteral("version")];
                if (v.isString())
                {
                    ver->setVersion(v.toString());
                }
                else if (v.isDouble())
                {
                    ver->setVersion(QString::number(v.toDouble()));
                }
                auto path = versionObj[QStringLiteral("path")].toArray();
                for (auto pathElem : path)
                {
                    ver->appendPath(pathElem.toString());
                }
                library->appendVersion(ver);
            }
        }
    }
    return true;
}

bool CodeInspectorApp::parseDefaultCompilerFromConfiguration(QJsonObject &obj)
{
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO;
#endif
    auto languages = obj.keys();
    m_defaultCompiler.clear();
    for (const auto &lang : languages)
    {
        m_defaultCompiler.insert(lang, obj[lang].toString());
    }

    return !m_defaultCompiler.empty();
}

const QString &CodeInspectorApp::getLanguageId(const QString &name)
{
    if (name.isEmpty())
    {
        return name;
    }
    auto iter = std::find_if(m_languageList.begin(), m_languageList.end(), [&name](const LanguagePtr &lang) { return lang->name == name; });
    Q_ASSERT(m_languageList.end() != iter);
    return (*iter)->id;
}

const QString &CodeInspectorApp::getLanguageName(const QString &id)
{
    if (id.isEmpty())
    {
        return id;
    }
    auto iter = std::find_if(m_languageList.begin(), m_languageList.end(), [&id](const LanguagePtr &lang) { return lang->id == id; });
    Q_ASSERT(m_languageList.end() != iter);
    return (*iter)->name;
}

const QString &CodeInspectorApp::getCompilerId(CompilerListPtr compilerList, const QString &name)
{
    if (name.isEmpty())
    {
        return name;
    }
    auto iter = std::find_if(compilerList->begin(), compilerList->end(), [&name](const CompilerPtr &compiler) { return compiler->name == name; });
    Q_ASSERT(compilerList->end() != iter);
    return (*iter)->id;
}

QStringList CodeInspectorApp::getExampleList(const QString &languageName)
{
    auto languageId = getLanguageId(languageName);
    QDir dir(":/resource/example/" + languageId);
    return dir.entryList();
}

QString CodeInspectorApp::getExampleContent(const QString &languageName, const QString &exampleName)
{
    auto  languageId = getLanguageId(languageName);
    QFile file(QString(":/resource/example/%1/%2").arg(languageId, exampleName));
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray content = file.readAll();
        file.close();
        return content;
    }
    return {};
}

const QString &CodeInspectorApp::getDefaultCompilerName(const QString &languageName)
{
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO << languageName;
#endif
    static QString emptyString;
    auto           languageId = getLanguageId(languageName);
    auto           langIt     = m_defaultCompiler.find(languageId);
    if (m_defaultCompiler.end() == langIt)
    {
        return emptyString;
    }
    auto compilerId   = langIt.value();
    auto compilerList = getCompilerList(languageName);
    auto iter =
        std::find_if(compilerList->begin(), compilerList->end(), [&compilerId](const CompilerPtr &compiler) { return compiler->id == compilerId; });
    if (compilerList->end() == iter)
    {
        return emptyString;
    }
    return (*iter)->name;
}

void CodeInspectorApp::requestConfigurations()
{
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO;
#endif
    quint64 value = QRandomGenerator::global()->generate64();
    QUrl    requestUrl("https://cdn.jsdelivr.net/gh/missdeer/codeinspector@master/core/resource/configurations.json?hash=" + QString::number(value));
    QNetworkRequest request = NetworkRequestHelper::NewRequest(requestUrl);

    auto *reply       = m_nam.get(request);
    auto *replyHelper = new NetworkReplyHelper(reply);
    replyHelper->setTimeout(networkRequestTimeout);
    connect(replyHelper, &NetworkReplyHelper::done, this, &CodeInspectorApp::onConfigurationRequestFinished);
}

bool CodeInspectorApp::storeConfiguration(const QByteArray &content)
{
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO;
#endif
    const QString dirPath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir          dir(dirPath);
    if (!dir.exists())
    {
        if (!dir.mkpath(dirPath))
        {
#if defined(LOGS_ENABLED)
            qDebug() << "creating directory failed:" << d;
#endif
            return false;
        }
    }
    const QString path = dirPath + "/configurations.json";
    QFile         file(path);
    if (!file.open(QIODevice::WriteOnly))
    {
#if defined(LOGS_ENABLED)
        qDebug() << "open file " << path << " for writting failed";
#endif
        return false;
    }
    file.write(content);
    file.close();
    return true;
}

bool CodeInspectorApp::loadConfigurationRawContent(const QString &path, QByteArray &content)
{
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO;
#endif
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
#if defined(LOGS_ENABLED)
        qDebug() << "open file " << path << " for reading failed";
#endif
        return false;
    }
#if defined(LOGS_ENABLED)
    qDebug() << "load configurations from" << path;
#endif
    content = file.readAll();
    file.close();
    return true;
}

bool CodeInspectorApp::parseConfiguration(const QByteArray &content)
{
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO;
#endif
    QJsonDocument doc = QJsonDocument::fromJson(content);
    if (!doc.isObject())
    {
#if defined(LOGS_ENABLED)
        qDebug() << "configuration is expected to be an object:" << content;
#endif
        return false;
    }

    QJsonObject obj       = doc.object();
    QJsonObject languages = obj[QStringLiteral("languages")].toObject();
    bool        ret       = parseLanguageListFromConfiguration(languages);
    QJsonObject libs      = obj[QStringLiteral("libs")].toObject();
    ret &= parseLibListFromConfiguration(libs);
    QJsonArray compilers = obj[QStringLiteral("compilers")].toArray();
    ret &= parseCompilerListFromConfiguration(compilers);
    QJsonObject defaultCompiler = obj[QStringLiteral("defaultCompiler")].toObject();
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
    auto iter =
        std::find_if(m_languageList.begin(), m_languageList.end(), [&language](const LanguagePtr &langElem) { return langElem->name == language; });
    Q_ASSERT(m_languageList.end() != iter);
#if defined(LOGS_ENABLED)
    qDebug() << "get example:" << (*iter)->example;
#endif
    return (*iter)->example;
}

void CodeInspectorApp::onLanguageListRequestFinished()
{
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO;
#endif
    auto *reply = qobject_cast<NetworkReplyHelper *>(sender());
    Q_ASSERT(reply);
    reply->deleteLater();

    QByteArray &content = reply->content();
    storeLanguageList(content);
    if (parseLanguageListFromJSON(content))
    {
        emit languageListReady();
    }
}

void CodeInspectorApp::onConfigurationRequestFinished()
{
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO;
#endif
    auto *reply = qobject_cast<NetworkReplyHelper *>(sender());
    reply->deleteLater();

    QByteArray &content = reply->content();
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO << QString(content).left(256);
#endif
    if (parseConfiguration(content))
    {
        storeConfiguration(content);
    }
}

void CodeInspectorApp::switchLanguage(const QString &language)
{
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO << m_compilerMap.size();
#endif
    auto iter = m_compilerMap.find(language);
    if (m_compilerMap.end() != iter && !iter.value()->isEmpty())
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
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO;
#endif
    auto *reply = qobject_cast<NetworkReplyHelper *>(sender());
    reply->deleteLater();

    QByteArray &content = reply->content();
    QString     name    = reply->data().toString();
    if (!content.isEmpty())
    {
        storeCompilerList(name, content);

        if (!parseCompilerListFromJSON(name, content))
        {
            switchLanguage(name);
        }
    }
    else
    {
        // m_compileOutput = reply->getErrorMessage();
    }
}

bool CodeInspectorApp::canCompile(const QString &language, const QString &compiler)
{
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO << language << compiler;
#endif
    if (m_compilerMap.find(language) == m_compilerMap.end())
    {
        return false;
    }
    auto compilerList = m_compilerMap.find(language).value();
    auto iter         = std::find_if(
        compilerList->begin(), compilerList->end(), [&compiler](const CompilerPtr &compilerElem) { return compilerElem->name == compiler; });
    return iter != compilerList->end();
}
