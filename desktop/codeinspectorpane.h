#ifndef CODEINSPECTORPANE_H
#define CODEINSPECTORPANE_H

#include <QWidget>
#include "compiler.hpp"
#include "compilerinfo.hpp"

QT_BEGIN_NAMESPACE
class QPushButton;
class QComboBox;
class QLineEdit;
class QSplitter;
class QTimer;
QT_END_NAMESPACE

class CodeInspectorTabWidget;
class OutputWindow;
class GodboltAgent;
class CodeEditor;

class CodeInspectorPane : public QWidget
{
    Q_OBJECT
public:
    explicit CodeInspectorPane(CodeEditor* codeEditor, QWidget *parent = nullptr);
    void initialize();
    void setCompilerList(CompilerListPtr cl);
    void setCurrentLanguage(const QString& languageName);
signals:
    void currentCompilerChanged(QString);
    void currentCompilerArgumentsChanged();
public slots:
    
private slots:
    void onNeedCompile();
    void onCompiled();
    void onDelayCompile();
    void onToggleOutput();
    void onActionBinaryTriggered();
    void onActionLabelTriggered();
    void onActionFunctionsTriggered();
    void onActionDirectivesTriggered();
    void onActionCommentsTriggered();
    void onActionTrimTriggered();
    void onActionIntelTriggered();
    void onActionDemangleTriggered();
    void onCurrentCompilerArgumentsChanged();
    void onCurrentCompilerChanged(const QString& compilerName);
private:
    CodeEditor *m_codeEditor;
    QComboBox *m_compilerList;
    QLineEdit *m_compilerArguments;
    QPushButton *m_btnToggleOutput;
    QPushButton *m_btnInspectorOptions;
    CodeInspectorTabWidget *m_codeInspectorTabWidget;
    OutputWindow *m_output;
    GodboltAgent *m_backend;
    QSplitter *m_splitter;
    QTimer *m_timer;
    
    QAction *m_actionBinrary;
    QAction *m_actionLabel;
    QAction *m_actionFunctions;
    QAction *m_actionDirectives;
    QAction *m_actionComments;
    QAction *m_actionTrim;
    QAction *m_actionIntel;
    QAction *m_actionDemangle;
    
    QString m_languageName;
    
    void storeToCache(const QString &name, const CompileInfo &ci);
    bool restoreFromCache(const QString& name, CompileInfo &ci);
    void showOutputWindow(bool show);
};

#endif // CODEINSPECTORPANE_H
