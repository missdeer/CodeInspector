#include "stdafx.h"
#include "settings.h"

Settings* g_settings = nullptr;

Settings::Settings()
{

}

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
    QSettings settings;
    settings.setValue("codeEditorFontFamily", m_codeEditorFontFamily);
    settings.setValue("codeEditorTheme", m_codeEditorTheme);
    settings.setValue("codeInspectorFontFamily", m_codeInspectorFontFamily);
    settings.setValue("codeInspectorTheme", m_codeInspectorTheme);
    settings.sync();
}

void Settings::load()
{
    QSettings settings;
    m_codeEditorFontFamily = settings.value("codeEditorFontFamily", QVariant("Source Code Pro")).toString();
    m_codeEditorTheme = settings.value("codeEditorTheme", QVariant("Default")).toString();
    m_codeInspectorFontFamily = settings.value("codeInspectorFontFamily", QVariant("Source Code Pro")).toString();
    m_codeInspectorTheme = settings.value("codeInspectorTheme", QVariant("Default")).toString();
}

const QString & Settings::codeEditorFontFamily() const
{
    return m_codeEditorFontFamily;
}

void Settings::setCodeEditorFontFamily(const QString &codeEditorFontFamily)
{
    m_codeEditorFontFamily = codeEditorFontFamily;
}

const QString & Settings::codeEditorTheme() const
{
    return m_codeEditorTheme;
}

void Settings::setCodeEditorTheme(const QString &codeEditorTheme)
{
    m_codeEditorTheme = codeEditorTheme;
}

const QString & Settings::codeInspectorFontFamily() const
{
    return m_codeInspectorFontFamily;
}

void Settings::setCodeInspectorFontFamily(const QString &codeInspectorFontFamily)
{
    m_codeInspectorFontFamily = codeInspectorFontFamily;
}

const QString & Settings::codeInspectorTheme() const
{
    return m_codeInspectorTheme;
}

void Settings::setCodeInspectorTheme(const QString &codeInspectorTheme)
{
    m_codeInspectorTheme = codeInspectorTheme;
}




