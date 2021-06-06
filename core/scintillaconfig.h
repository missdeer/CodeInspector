#ifndef SCINTILLACONFIG_H
#define SCINTILLACONFIG_H

#include <QString>

class ScintillaEdit;
class QDomElement;

class ScintillaConfig
{
public:
    explicit ScintillaConfig(ScintillaEdit *sci) : m_sci(sci) {}
    void initScintilla();
    void initEditorFolderStyle();
    void initLexerStyle(const QString &lang);
    void initEditorMargins();
    void initInspectorMargins(bool binary);
    void inttOuputWindowMargins();
    void initMarkers();

private:
    ScintillaEdit *m_sci {nullptr};

    void applyStyle(const QDomElement &styleElem);
    void applyGlobalStyle(const QDomElement &styleElem);
    void applyLexer(const QString &configPath, const QString &lang);
    void applyThemeStyle(const QString &themePath, const QString &lang);
    void extractScintilluaLexers(const QString &lexersPath);
};

#endif // SCINTILLACONFIG_H
