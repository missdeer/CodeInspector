#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include "ScintillaEdit.h"
#include "scintillaconfig.h"

class CodeEditor : public ScintillaEdit
{
    Q_OBJECT
public:
    explicit CodeEditor(QWidget *parent = 0);
    ~CodeEditor();
    void initialize();
    void setLanguage(const QString& lang);
    void setContent(const QString& content);
    void setContent(const QByteArray& content);
    void clearContent();
signals:
    void contentModified();
private slots:
    void linesAdded(int linesAdded);
    void marginClicked(int position, int modifiers, int margin);
    void modified(int type, int position, int length, int linesAdded,
                  const QByteArray &text, int line, int foldNow, int foldPrev);
private:
    ScintillaConfig m_sc;
};

#endif // CODEEDITOR_H
