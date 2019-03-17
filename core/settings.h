#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

class Settings
{
public:
    Settings();
    Settings(const Settings&) = delete;
    void operator=(const Settings&) = delete;
    Settings(Settings&&) = delete;
    void operator=(Settings&&) = delete;
    ~Settings();

    void save();
    void load();
    void initialize();

    const QString & codeEditorFontFamily() const;
    void setCodeEditorFontFamily(const QString &codeEditorFontFamily);

    const QString & codeEditorTheme() const;
    void setCodeEditorTheme(const QString &codeEditorTheme);

    const QString & codeInspectorFontFamily() const;
    void setCodeInspectorFontFamily(const QString &codeInspectorFontFamily);

    const QString & codeInspectorTheme() const;
    void setCodeInspectorTheme(const QString &codeInspectorTheme);

    bool rememberLastSession() const;
    void setRememberLastSession(bool rememberLastSession);

    int defaultLanguageIndex() const;
    void setDefaultLanguageIndex(int defaultLanguageIndex);

    int autoRefreshInterval() const;
    void setAutoRefreshInterval(int autoRefreshInterval);

    int editorZoomFactor() const;
    void setEditorZoomFactor(int editorZoomFactor);

private:
    bool m_rememberLastSession;
    int m_defaultLanguageIndex;
    int m_autoRefreshInterval; // millisecond
    int m_editorZoomFactor; // percent

    QString m_codeEditorFontFamily;
    QString m_codeEditorTheme;
    QString m_codeInspectorFontFamily;
    QString m_codeInspectorTheme;
};

extern Settings *g_settings;

#endif // SETTINGS_H
