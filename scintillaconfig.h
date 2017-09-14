#ifndef SCINTILLACONFIG_H
#define SCINTILLACONFIG_H

#include <QString>

class ScintillaEdit;
class QDomElement;

class ScintillaConfig
{
public:
    void initScintilla(ScintillaEdit* sci);
    void initEditorFolderStyle(ScintillaEdit* sci);
    void initLexerStyle(ScintillaEdit* sci, const QString &lang);
    void initEditorMargins(ScintillaEdit* sci);
    void initInspectorMargins(ScintillaEdit* sci, bool binary);

    void applyLanguageStyle(ScintillaEdit* sci, const QString& configPath, const QString& lang);
    void applyThemeStyle(ScintillaEdit* sci, const QString& themePath, const QString &lang);
    QString matchPatternLanguage(const QString &filename);
private:
    bool matchPattern(const QString &filename, const QString &pattern);
    bool matchSuffix(const QString &filename, const QString &suffix);
    void applyStyle(ScintillaEdit* sci, const QDomElement& styleElem);
};

#endif // SCINTILLACONFIG_H
