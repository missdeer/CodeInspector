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
    addTab(new CodeInspector(this), tr("Inspector"));
    addTab(new LLVMMachineCodeAnalyzerOutput(this), tr("LLVM MCA"));
    addTab(new ASTOutput(this), tr("AST"));
    addTab(new OptimizationOutput(this), tr("Optimization"));
    addTab(new GCCTreeRTLOutput(this), tr("GCC Tree/RTL Output"));
}
