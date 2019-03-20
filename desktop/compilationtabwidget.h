#ifndef COMPILATIONTABWIDGET_H
#define COMPILATIONTABWIDGET_H

#include <QTabWidget>

class CodeEditor;

class CompilationTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit CompilationTabWidget(CodeEditor* codeEditor, QWidget* parent = nullptr);
public slots:
    void languageChanged(const QString &languageName);
    void currentCompilerChanged(const QString &compilerName);
    
private:
    CodeEditor* m_codeEditor;
    void addCodeInspectorPaneTab();
};

#endif // COMPILATIONTABWIDGET_H
