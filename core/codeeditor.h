#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include "ScintillaEdit.h"
#include "scintillaconfig.h"

class CodeEditor : public ScintillaEdit
{
    Q_OBJECT
public:
    explicit CodeEditor(QWidget *parent = nullptr);
    ~CodeEditor() override = default;
    void initialize();
    void setLanguage(const QString &lang);
    void setContent(const QString &content);
    void setContent(const QByteArray &content);
    void clearContent();
    void setMarkerColor(const QMap<int, intptr_t> &markerColor);
signals:
    void contentModified();
private slots:
    void onLinesAdded(Scintilla::Position linesAdded);
    void onMarginClicked(Scintilla::Position position, Scintilla::KeyMod modifiers, int margin);
    void onModified(Scintilla::ModificationFlags type,
                    Scintilla::Position          position,
                    Scintilla::Position          length,
                    Scintilla::Position          linesAdded,
                    const QByteArray &           text,
                    Scintilla::Position          line,
                    Scintilla::FoldLevel         foldNow,
                    Scintilla::FoldLevel         foldPrev);

private:
    ScintillaConfig m_sc;
};

#endif // CODEEDITOR_H
