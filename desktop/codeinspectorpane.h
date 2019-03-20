#ifndef CODEINSPECTORPANE_H
#define CODEINSPECTORPANE_H

#include <QWidget>
#include "compiler.hpp"

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

class CodeInspectorPane : public QWidget
{
    Q_OBJECT
public:
    explicit CodeInspectorPane(QWidget *parent = nullptr);
    void initialize();
    void setCompilerList(CompilerListPtr cl);
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
private:
    QComboBox* m_compilerList;
    QLineEdit* m_compilerArguments;
    QPushButton* m_btnToggleOutput;
    QPushButton* m_btnInspectorOptions;
    CodeInspectorTabWidget* m_codeInspectorTabWidget;
    OutputWindow* m_output;
    GodboltAgent* m_backend;
    QSplitter *m_splitter;
    QTimer* m_timer;
};

#endif // CODEINSPECTORPANE_H
