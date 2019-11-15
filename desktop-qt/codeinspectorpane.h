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
    explicit CodeInspectorPane(CodeEditor *codeEditor, QWidget *parent = nullptr);
    void initialize();
    void setCompilerList(CompilerListPtr cl);
    void setCurrentLanguage(const QString &languageName);
signals:
    void currentCompilerChanged(QString);
    void currentCompilerArgumentsChanged();
public slots:

private slots:
    void onNeedCompile();
    void onCompiled();
    void onHasGccDumpOutput();
    void onHasLLVMMCAOutput();
    void onHasOptimizationOutput();
    void onHasPaholeOutput();
    void onHasClangTidyOutput();
    void onHasASTOutput();
    void onHasLLVMIROutput();
    void onHasLddOutput();
    void onHasX86To6502Output();
    void onHasReadElfOutput();
    void onHasIncludeWhatYouUseOutput();
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
    void onCurrentCompilerChanged(const QString &compilerName);

    void onRequestLLVMMCA();
    void onRequestAST();
    void onRequestLLVMIR();
    void onRequestOptimization();
    void onRequestGCCTreeRTL();
    void onRequestPahole();
    void onRequestClangTidy();
    void onRequestClangQuery();
    void onRequestLdd();
    void onRequestReadElf();
    void onRequestX86To6502();
    void onRequestIncludeWhatYouUse();
    void onRefreshGCCDumpOutput(QString pass, bool gccTree, bool rtl);

private:
    CodeEditor *            m_codeEditor;
    QComboBox *             m_compilerList;
    QLineEdit *             m_compilerArguments;
    QPushButton *           m_btnToggleOutput;
    CodeInspectorTabWidget *m_codeInspectorTabWidget;
    OutputWindow *          m_output;
    GodboltAgent *          m_backend;
    QSplitter *             m_splitter;
    QTimer *                m_timer;

    QPushButton *m_btnBinrary;
    QPushButton *m_btnLabel;
    QPushButton *m_btnFunctions;
    QPushButton *m_btnDirectives;
    QPushButton *m_btnComments;
    QPushButton *m_btnTrim;
    QPushButton *m_btnIntel;
    QPushButton *m_btnDemangle;

    QString m_languageName;

    QMap<int, intptr_t> m_markerMap;

    void storeToCache(const QString &name, const CompileInfo &ci);
    bool restoreFromCache(const QString &name, CompileInfo &ci);
    void showOutputWindow(bool show);
};

#endif // CODEINSPECTORPANE_H
