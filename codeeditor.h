#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include "ScintillaEdit.h"
#include "scintillaconfig.h"

class CodeEditor : public ScintillaEdit
{
    Q_OBJECT
public:
    explicit CodeEditor(QWidget *parent = 0);
    void initialize();
    void setLanguage(const QString& lang);
    void setContent(const QString& content);
private slots:
    void linesAdded(int linesAdded);
    void marginClicked(int position, int modifiers, int margin);
private:
    ScintillaConfig m_sc;
};

#endif // CODEEDITOR_H
