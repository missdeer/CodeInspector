#ifndef CODEINSPECTORPANE_H
#define CODEINSPECTORPANE_H

#include <QWidget>
#include "compiler.hpp"

QT_BEGIN_NAMESPACE
class QPushButton;
class QComboBox;
class QLineEdit;
class QSplitter;
QT_END_NAMESPACE

class CodeInspectorTabWidget;
class OutputWindow;
class GodboltAgent;
class PopupMenuToolButton;

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
    void onToggleOutput();
    void onActionBinaryTriggered();
    void onActionLabelTriggered();
    void onActionFunctionsTriggered();
    void onActionDirectivesTriggered();
    void onActionCommentsTriggered();
    void onActionTrimTriggered();
    void onActionIntelTriggered();
    void onActionDemangleTriggered();
private:
    QComboBox* m_compilerList;
    QLineEdit* m_compilerArguments;
    QPushButton* m_btnToggleOutput;
    PopupMenuToolButton* m_btnInspectorOptions;
    CodeInspectorTabWidget* m_codeInspectorTabWidget;
    OutputWindow* m_output;
    GodboltAgent* m_backend;
    QSplitter *m_splitter;
};

#endif // CODEINSPECTORPANE_H
