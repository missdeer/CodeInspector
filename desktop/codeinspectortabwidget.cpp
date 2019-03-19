#include "stdafx.h"
#include "codeinspector.h"
#include "gcctreertloutput.h"
#include "llvmmachinecodeanalyzeroutput.h"
#include "astoutput.h"
#include "optimizationoutput.h"
#include "codeinspectortabwidget.h"

CodeInspectorTabWidget::CodeInspectorTabWidget(QWidget *parent)
    : QTabWidget (parent)
{
    setTabPosition(East);
    addTab(new CodeInspector(this), QIcon(":/resource/image/tab/inspector.png"), tr("Inspector"));
    addTab(new LLVMMachineCodeAnalyzerOutput(this), QIcon(":/resource/image/tab/llvm.png"), tr("LLVM MCA"));
    addTab(new ASTOutput(this), QIcon(":/resource/image/tab/ast.png"), tr("AST"));
    addTab(new OptimizationOutput(this), QIcon(":/resource/image/tab/optimization.png"), tr("Optimization"));
    addTab(new GCCTreeRTLOutput(this), QIcon(":/resource/image/tab/gcc.png"), tr("GCC Tree/RTL Output"));
}
