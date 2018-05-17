#include "stdafx.h"
#include <QSettings>
#include "quickwidgetapi.h"

QuickWidgetAPI::QuickWidgetAPI(QObject *parent)
    : QObject(parent)
    , m_binary(false)
    , m_binaryEnabled(true)
    , m_labels(true)
    , m_labelsEnabled(true)
    , m_trim(true)
    , m_trimEnabled(true)
    , m_directives(true)
    , m_directivesEnabled(true)
    , m_intel(true)
    , m_intelEnabled(true)
    , m_commentOnly(true)
    , m_commentOnlyEnabled(true)
    , m_demangle(true)
    , m_demangleEnabled(true)
{
    QSettings settings;
    m_rememberLastSession = settings.value("rememberLastSession", true).toBool();
    m_defaultLanguageIndex = settings.value("defaultLanguageIndex", 0).toInt();
#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
    m_autoRefreshInterval = settings.value("autoRefreshInterval", 1500).toInt();
#else
    m_autoRefreshInterval = settings.value("autoRefreshInterval", 750).toInt();
#endif
    m_editorZoomFactor = settings.value("editorZoomFactor", 100).toInt();
}

void QuickWidgetAPI::closeConfiguration()
{
    emit doCloseConfiguration();
}

void QuickWidgetAPI::loadExample(int index)
{
    QString name = m_examples.at(index);
    emit doLoadExample(name);
}

void QuickWidgetAPI::librarySwitched()
{
    emit doLibrarySwitched();
}

bool QuickWidgetAPI::binary() const
{
    return m_binary;
}

void QuickWidgetAPI::setBinary(bool binary)
{
    if (m_binary == binary)
        return;
    m_binary = binary;
    emit binaryChanged();        
    qDebug() << __FUNCTION__ << binary;
}

bool QuickWidgetAPI::labels() const
{
    return m_labels;
}

void QuickWidgetAPI::setLabels(bool labels)
{
    if (m_labels == labels)
        return;
    m_labels = labels;
    emit labelsChanged();
    qDebug() << __FUNCTION__ << labels;
}

bool QuickWidgetAPI::trim() const
{
    return m_trim;
}

void QuickWidgetAPI::setTrim(bool trim)
{
    if (m_trim == trim)
        return;
    m_trim = trim;
    emit trimChanged();
    qDebug() << __FUNCTION__ << trim;
}

bool QuickWidgetAPI::directives() const
{
    return m_directives;
}

void QuickWidgetAPI::setDirectives(bool directives)
{
    if (m_directives == directives)
        return;
    m_directives = directives;
    emit directivesChanged();
    qDebug() << __FUNCTION__ << directives;
}

bool QuickWidgetAPI::intel() const
{
    return m_intel;
}

void QuickWidgetAPI::setIntel(bool intel)
{
    if (m_intel == intel)
        return;
    m_intel = intel;
    emit intelChanged();
    qDebug() << __FUNCTION__ << intel;
}

bool QuickWidgetAPI::commentOnly() const
{
    return m_commentOnly;
}

void QuickWidgetAPI::setCommentOnly(bool commentOnly)
{
    if (m_commentOnly == commentOnly)
        return;
    m_commentOnly = commentOnly;
    emit commentOnlyChanged();
    qDebug() << __FUNCTION__ << commentOnly;
}

bool QuickWidgetAPI::binaryEnabled() const
{
    return m_binaryEnabled;
}

void QuickWidgetAPI::setBinaryEnabled(bool binaryEnabled)
{
    if (m_binaryEnabled == binaryEnabled)
        return;
    if (!binaryEnabled)
        m_binary = false;
    m_binaryEnabled = binaryEnabled;
    emit binaryEnabledChanged();
}

bool QuickWidgetAPI::labelsEnabled() const
{
    return m_labelsEnabled;
}

void QuickWidgetAPI::setLabelsEnabled(bool labelsEnabled)
{
    if (m_labelsEnabled == labelsEnabled)
        return;
    m_labelsEnabled = labelsEnabled;
    emit labelsEnabledChanged();
}

bool QuickWidgetAPI::trimEnabled() const
{
    return m_trimEnabled;
}

void QuickWidgetAPI::setTrimEnabled(bool trimEnabled)
{
    if (m_trimEnabled == trimEnabled)
        return;
    m_trimEnabled = trimEnabled;
    emit trimEnabledChanged();
}

bool QuickWidgetAPI::directivesEnabled() const
{
    return m_directivesEnabled;
}

void QuickWidgetAPI::setDirectivesEnabled(bool directivesEnabled)
{
    if (m_directivesEnabled == directivesEnabled)
        return;
    m_directivesEnabled = directivesEnabled;
    emit directivesEnabledChanged();
}

bool QuickWidgetAPI::intelEnabled() const
{
    return m_intelEnabled;
}

void QuickWidgetAPI::setIntelEnabled(bool intelEnabled)
{
    if (m_intelEnabled == intelEnabled)
        return;
    if (!intelEnabled)
        m_intel = false;
    m_intelEnabled = intelEnabled;
    emit intelEnabledChanged();
}

bool QuickWidgetAPI::commentOnlyEnabled() const
{
    return m_commentOnlyEnabled;
}

void QuickWidgetAPI::setCommentOnlyEnabled(bool commentOnlyEnabled)
{
    if (m_commentOnlyEnabled == commentOnlyEnabled)
        return;
    m_commentOnlyEnabled = commentOnlyEnabled;
    emit commentOnlyEnabledChanged();
}

bool QuickWidgetAPI::demangle() const
{
    return m_demangle;
}

void QuickWidgetAPI::setDemangle(bool demangle)
{
    if (m_demangle == demangle)
        return;
    m_demangle = demangle;
    emit demangleChanged();
    qDebug() << __FUNCTION__ << demangle;
}

bool QuickWidgetAPI::demangleEnabled() const
{
    return m_demangleEnabled;
}

void QuickWidgetAPI::setDemangleEnabled(bool demangleEnabled)
{
    if (m_demangleEnabled == demangleEnabled)
        return;
    if (!demangleEnabled)
        m_demangle = false;
    m_demangleEnabled = demangleEnabled;
    emit demangleEnabledChanged();
}

const QStringList& QuickWidgetAPI::examples() const
{
    return m_examples;
}

void QuickWidgetAPI::setExamples(const QStringList &examples)
{
    m_examples = examples;
    emit examplesChanged();
}

void QuickWidgetAPI::setLibs(LibraryListPtr libs)
{
    m_libs.swap(libs);
    emit libsChanged();
}

LibraryListPtr QuickWidgetAPI::libs()
{
    return m_libs;
}

QQmlListProperty<Library> QuickWidgetAPI::qmlListPropertyLibs()
{
    return QQmlListProperty<Library>(this, this,
                                     &QuickWidgetAPI::appendLib,
                                     &QuickWidgetAPI::libCount,
                                     &QuickWidgetAPI::lib,
                                     &QuickWidgetAPI::clearLibs
                                     );
}

const QStringList& QuickWidgetAPI::languages() const
{
    return m_languages;
}

void QuickWidgetAPI::setLanguages(const QStringList &languages)
{
    m_languages = languages;
    emit languagesChanged();
    for (int i =0; i < languages.size(); i++)
    {
        if (languages.at(i) == "C++")
        {
            m_defaultLanguageIndex = i;
            break;
        }
    }
}

bool QuickWidgetAPI::rememberLastSession() const
{
    return m_rememberLastSession;
}

void QuickWidgetAPI::setRememberLastSession(bool rememberLastSession)
{    
    QSettings settings;
    settings.setValue("rememberLastSession", rememberLastSession);
    settings.sync();
    m_rememberLastSession = rememberLastSession;
    emit rememberLastSessionChanged();
}

int QuickWidgetAPI::autoRefreshInterval() const
{
    return m_autoRefreshInterval;
}

void QuickWidgetAPI::setAutoRefreshInterval(int autoRefreshInterval)
{
    QSettings settings;
    settings.setValue("autoRefreshInterval", autoRefreshInterval);
    settings.sync();
    m_autoRefreshInterval = autoRefreshInterval;
    emit autoRefreshIntervalChanged();
}

int QuickWidgetAPI::editorZoomFactor() const
{
    return m_editorZoomFactor;
}

void QuickWidgetAPI::setEditorZoomFactor(int editorZoomFactor)
{
    QSettings settings;
    settings.setValue("editorZoomFactor", editorZoomFactor);
    settings.sync();
    m_editorZoomFactor = editorZoomFactor;
    emit editorZoomFactorChanged();
}

int QuickWidgetAPI::defaultLanguageIndex() const
{
    return m_defaultLanguageIndex;
}

void QuickWidgetAPI::setDefaultLanguageIndex(int defaultLanguageIndex)
{
    QSettings settings;
    settings.setValue("defaultLanguageIndex", defaultLanguageIndex);
    settings.sync();
    m_defaultLanguageIndex = defaultLanguageIndex;
    emit defaultLanguageIndexChanged();
}

void QuickWidgetAPI::appendLib(QQmlListProperty<Library> *list, Library *p)
{
    reinterpret_cast< QuickWidgetAPI* >(list->data)->appendLib(p);
}

int QuickWidgetAPI::libCount(QQmlListProperty<Library> *list)
{
    return reinterpret_cast< QuickWidgetAPI* >(list->data)->libCount();
}

Library *QuickWidgetAPI::lib(QQmlListProperty<Library> *list, int index)
{
    return reinterpret_cast< QuickWidgetAPI* >(list->data)->lib(index);
}

void QuickWidgetAPI::clearLibs(QQmlListProperty<Library> *list)
{
    reinterpret_cast< QuickWidgetAPI* >(list->data)->clearLibs();
}

void QuickWidgetAPI::appendLib(Library *) {}

int QuickWidgetAPI::libCount() const
{
    if (!m_libs)
        return 0;
    return m_libs->length();
}

Library *QuickWidgetAPI::lib(int index) const
{
    if (!m_libs)
        return nullptr;
    if (index >= m_libs->length())
        return nullptr;
    return m_libs->at(index).data();
}

void QuickWidgetAPI::clearLibs() {}
