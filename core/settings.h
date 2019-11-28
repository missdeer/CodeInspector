#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>

class Settings
{
public:
    Settings()                 = default;
    Settings(const Settings &) = delete;
    void operator=(const Settings &) = delete;
    Settings(Settings &&)            = delete;
    void operator=(Settings &&) = delete;
    ~Settings();

    void save();
    void load();
    void initialize();

    [[nodiscard]] const QString &codeEditorFontFamily() const;
    void                         setCodeEditorFontFamily(const QString &codeEditorFontFamily);

    [[nodiscard]] const QString &codeEditorTheme() const;
    void                         setCodeEditorTheme(const QString &codeEditorTheme);

    [[nodiscard]] const QString &codeInspectorFontFamily() const;
    void                         setCodeInspectorFontFamily(const QString &codeInspectorFontFamily);

    [[nodiscard]] const QString &codeInspectorTheme() const;
    void                         setCodeInspectorTheme(const QString &codeInspectorTheme);

    [[nodiscard]] bool rememberLastSession() const;
    void               setRememberLastSession(bool rememberLastSession);

    [[nodiscard]] int defaultLanguageIndex() const;
    void              setDefaultLanguageIndex(int defaultLanguageIndex);

    [[nodiscard]] int autoRefreshInterval() const;
    void              setAutoRefreshInterval(int autoRefreshInterval);

    [[nodiscard]] int editorZoomFactor() const;
    void              setEditorZoomFactor(int editorZoomFactor);

    [[nodiscard]] bool binary() const;
    void               setBinary(bool binary);

    [[nodiscard]] bool label() const;
    void               setLabel(bool label);

    [[nodiscard]] bool functions() const;
    void               setFunctions(bool functions);

    [[nodiscard]] bool directives() const;
    void               setDirectives(bool directives);

    [[nodiscard]] bool comments() const;
    void               setComments(bool comments);

    [[nodiscard]] bool trim() const;
    void               setTrim(bool trim);

    [[nodiscard]] bool intel() const;
    void               setIntel(bool intel);

    [[nodiscard]] bool demangle() const;
    void               setDemangle(bool demangle);

    [[nodiscard]] const QString &apiBaseURL() const;
    void                         setApiBaseURL(const QString &apiBaseURL);

    [[nodiscard]] bool sslRequired() const;
    void               setSslRequired(bool sslRequired);

private:
    bool m_rememberLastSession {true};
    bool m_binary {false};
    bool m_label {true};
    bool m_functions {true};
    bool m_directives {true};
    bool m_comments {true};
    bool m_trim {true};
    bool m_intel {true};
    bool m_demangle {true};
    bool m_sslRequired {true};
    int  m_defaultLanguageIndex {0};
    int  m_autoRefreshInterval {750}; // millisecond
    int  m_editorZoomFactor {100};    // percent

    QString m_codeEditorFontFamily;
    QString m_codeEditorTheme;
    QString m_codeInspectorFontFamily;
    QString m_codeInspectorTheme;
    QString m_apiBaseURL;
};

inline Settings *g_settings = nullptr;

#endif // SETTINGS_H
