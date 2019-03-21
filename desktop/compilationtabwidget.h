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
    void onLanguageChanged(const QString &languageName);
    void onCurrentCompilerChanged(const QString &compilerName);
private slots:
    void onCustomContextMenuRequested(const QPoint &pos);
    void addCodeInspectorPaneTab();
    void onTabCloseRequested(int index);
private:
    CodeEditor* m_codeEditor;
    QString m_currentLanguageName;
};

#endif // COMPILATIONTABWIDGET_H
