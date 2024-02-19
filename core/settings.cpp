#include <QSettings>

#include "settings.h"

Settings::~Settings()
{
    save();
}

void Settings::initialize()
{
    load();
}

void Settings::save()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "IsmISV", "CodeInspector");
    settings.setValue(QStringLiteral("rememberLastSession"), m_rememberLastSession);
    settings.setValue(QStringLiteral("autoRefreshInterval"), m_autoRefreshInterval);
    settings.setValue(QStringLiteral("editorZoomFactor"), m_editorZoomFactor);
    settings.setValue(QStringLiteral("defaultLanguageIndex"), m_defaultLanguageIndex);
    settings.setValue(QStringLiteral("codeEditorFontFamily"), m_codeEditorFontFamily);
    settings.setValue(QStringLiteral("codeEditorTheme"), m_codeEditorTheme);
    settings.setValue(QStringLiteral("codeInspectorFontFamily"), m_codeInspectorFontFamily);
    settings.setValue(QStringLiteral("codeInspectorTheme"), m_codeInspectorTheme);
    settings.setValue(QStringLiteral("binary"), m_binary);
    settings.setValue(QStringLiteral("label"), m_label);
    settings.setValue(QStringLiteral("functions"), m_functions);
    settings.setValue(QStringLiteral("directives"), m_directives);
    settings.setValue(QStringLiteral("comments"), m_comments);
    settings.setValue(QStringLiteral("trim"), m_trim);
    settings.setValue(QStringLiteral("intel"), m_intel);
    settings.setValue(QStringLiteral("demangle"), m_demangle);
    settings.setValue(QStringLiteral("apiBaseURL"), m_apiBaseURL);
    settings.setValue(QStringLiteral("sslRequired"), m_sslRequired);
    settings.sync();
}

void Settings::load()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "IsmISV", "CodeInspector");
    m_rememberLastSession  = settings.value(QStringLiteral("rememberLastSession"), true).toBool();
    m_binary               = settings.value(QStringLiteral("binary"), false).toBool();
    m_label                = settings.value(QStringLiteral("label"), true).toBool();
    m_functions            = settings.value(QStringLiteral("functions"), true).toBool();
    m_directives           = settings.value(QStringLiteral("directives"), true).toBool();
    m_comments             = settings.value(QStringLiteral("comments"), true).toBool();
    m_trim                 = settings.value(QStringLiteral("trim"), true).toBool();
    m_intel                = settings.value(QStringLiteral("intel"), true).toBool();
    m_demangle             = settings.value(QStringLiteral("demangle"), true).toBool();
    m_defaultLanguageIndex = settings.value(QStringLiteral("defaultLanguageIndex"), 0).toInt();
#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
    m_autoRefreshInterval = settings.value(QStringLiteral("autoRefreshInterval"), 1500).toInt();
#else
    m_autoRefreshInterval = settings.value(QStringLiteral("autoRefreshInterval"), 750).toInt();
#endif
    m_editorZoomFactor        = settings.value(QStringLiteral("editorZoomFactor"), 100).toInt();
    m_codeEditorFontFamily    = settings.value(QStringLiteral("codeEditorFontFamily"), QVariant("Source Code Pro")).toString();
    m_codeEditorTheme         = settings.value(QStringLiteral("codeEditorTheme"), QVariant("Default")).toString();
    m_codeInspectorFontFamily = settings.value(QStringLiteral("codeInspectorFontFamily"), QVariant("Source Code Pro")).toString();
    m_codeInspectorTheme      = settings.value(QStringLiteral("codeInspectorTheme"), QVariant("Default")).toString();
    m_apiBaseURL              = settings.value(QStringLiteral("apiBaseURL"), QVariant("https://compiler-explorer.com")).toString();
    m_sslRequired             = settings.value(QStringLiteral("sslRequired"), true).toBool();
}

const QString &Settings::codeEditorFontFamily() const
{
    return m_codeEditorFontFamily;
}

void Settings::setCodeEditorFontFamily(const QString &codeEditorFontFamily)
{
    m_codeEditorFontFamily = codeEditorFontFamily;
}

const QString &Settings::codeEditorTheme() const
{
    return m_codeEditorTheme;
}

void Settings::setCodeEditorTheme(const QString &codeEditorTheme)
{
    m_codeEditorTheme = codeEditorTheme;
}

bool Settings::rememberLastSession() const
{
    return m_rememberLastSession;
}

void Settings::setRememberLastSession(bool rememberLastSession)
{
    m_rememberLastSession = rememberLastSession;
}

int Settings::defaultLanguageIndex() const
{
    return m_defaultLanguageIndex;
}

void Settings::setDefaultLanguageIndex(int defaultLanguageIndex)
{
    m_defaultLanguageIndex = defaultLanguageIndex;
}

int Settings::autoRefreshInterval() const
{
    return m_autoRefreshInterval;
}

void Settings::setAutoRefreshInterval(int autoRefreshInterval)
{
    m_autoRefreshInterval = autoRefreshInterval;
}

int Settings::editorZoomFactor() const
{
    return m_editorZoomFactor;
}

void Settings::setEditorZoomFactor(int editorZoomFactor)
{
    m_editorZoomFactor = editorZoomFactor;
}

bool Settings::binary() const
{
    return m_binary;
}

void Settings::setBinary(bool binary)
{
    m_binary = binary;
}

bool Settings::label() const
{
    return m_label;
}

void Settings::setLabel(bool label)
{
    m_label = label;
}

bool Settings::functions() const
{
    return m_functions;
}

void Settings::setFunctions(bool functions)
{
    m_functions = functions;
}

bool Settings::directives() const
{
    return m_directives;
}

void Settings::setDirectives(bool directives)
{
    m_directives = directives;
}

bool Settings::comments() const
{
    return m_comments;
}

void Settings::setComments(bool comments)
{
    m_comments = comments;
}

bool Settings::trim() const
{
    return m_trim;
}

void Settings::setTrim(bool trim)
{
    m_trim = trim;
}

bool Settings::intel() const
{
    return m_intel;
}

void Settings::setIntel(bool intel)
{
    m_intel = intel;
}

bool Settings::demangle() const
{
    return m_demangle;
}

void Settings::setDemangle(bool demangle)
{
    m_demangle = demangle;
}

const QString &Settings::apiBaseURL() const
{
    return m_apiBaseURL;
}

void Settings::setApiBaseURL(const QString &apiBaseURL)
{
    m_apiBaseURL = apiBaseURL;
}

bool Settings::sslRequired() const
{
    return m_sslRequired;
}

void Settings::setSslRequired(bool sslRequired)
{
    m_sslRequired = sslRequired;
}
