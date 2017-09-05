#ifndef SETTINGS_H
#define SETTINGS_H

class Settings
{
public:
    Settings();
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

private:
    QString m_codeEditorFontFamily;
    QString m_codeEditorTheme;
    QString m_codeInspectorFontFamily;
    QString m_codeInspectorTheme;
};

extern Settings *g_settings;

#endif // SETTINGS_H
