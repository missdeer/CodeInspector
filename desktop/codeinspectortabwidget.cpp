#include "stdafx.h"
#include <QTabBar>
#include "codeinspector.h"
#include "gcctreertloutput.h"
#include "llvmmachinecodeanalyzeroutput.h"
#include "astoutput.h"
#include "optimizationoutput.h"
#include "codeinspectortabwidget.h"

CodeInspectorTabWidget::CodeInspectorTabWidget(QWidget *parent)
    : QTabWidget (parent)
    , m_codeInspector(new CodeInspector(this))
    , m_llvmMCA(nullptr)
    , m_ast(nullptr)
    , m_optimization(nullptr)
    , m_gccTreeRTL(nullptr)
{
    setTabPosition(East);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, this, &CodeInspectorTabWidget::onCustomContextMenuRequested);
    addTab(m_codeInspector, QIcon(":/resource/image/tab/inspector.png"), tr("Inspector"));
}

QMap<int, sptr_t> CodeInspectorTabWidget::setCodeInspectorAsmItems(const AsmItemList &items, bool binary)
{
    Q_ASSERT(m_codeInspector);
    return m_codeInspector->setAsmItems(items, binary);
}

void CodeInspectorTabWidget::setCodeInspectorContent(const QString &content, bool binary)
{
    Q_ASSERT(m_codeInspector);
    m_codeInspector->setContent(content, binary);
}

void CodeInspectorTabWidget::setLLVMMACContent(const QString &content)
{
    if (!m_llvmMCA)
    {
        m_llvmMCA = new LLVMMachineCodeAnalyzerOutput(this);
        addTab(m_llvmMCA, QIcon(":/resource/image/tab/llvm.png"), tr("LLVM MCA"));
    }
    Q_ASSERT(m_llvmMCA);
    m_llvmMCA->setContent(content);
}

void CodeInspectorTabWidget::setASTContent(const QString &content)
{
    if (!m_ast)
    {
        m_ast = new ASTOutput(this);
        addTab(m_ast, QIcon(":/resource/image/tab/ast.png"), tr("AST"));
    }
    Q_ASSERT(m_ast);
    m_ast->setContent(content);
}

void CodeInspectorTabWidget::setOptimizationContent(const QString &content)
{
    if (!m_optimization)
    {
        m_optimization = new OptimizationOutput(this);
        addTab(m_optimization, QIcon(":/resource/image/tab/optimization.png"), tr("Optimization"));
    }
    Q_ASSERT(m_optimization);
    m_optimization->setContent(content);
}

void CodeInspectorTabWidget::setGCCTreeRTLContent(const QString &content)
{
    if (!m_gccTreeRTL)
    {
        m_gccTreeRTL = new GCCTreeRTLOutput(this);
        addTab(m_gccTreeRTL, QIcon(":/resource/image/tab/gcc.png"), tr("GCC Tree/RTL Output"));
    }
    Q_ASSERT(m_gccTreeRTL);
    m_gccTreeRTL->setContent(content);
}

void CodeInspectorTabWidget::onCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    
    QAction* pOptimizationAction = new QAction(QIcon(":/resource/image/tab/optimization.png"), tr("Optimization Output"), &menu);
    connect(pOptimizationAction, &QAction::triggered, this, &CodeInspectorTabWidget::requestOptimization);
    menu.addAction(pOptimizationAction);
    
    QAction* pASTAction = new QAction(QIcon(":/resource/image/tab/ast.png"), tr("AST"), &menu);
    connect(pASTAction, &QAction::triggered, this, &CodeInspectorTabWidget::requestAST);
    menu.addAction(pASTAction);
    
    QAction* pGCCTreeRTLAction = new QAction(QIcon(":/resource/image/tab/gcc.png"), tr("GCC Tree/RTL Output"), &menu);
    connect(pGCCTreeRTLAction, &QAction::triggered, this, &CodeInspectorTabWidget::requestGCCTreeRTL);
    menu.addAction(pGCCTreeRTLAction);
    
    QAction* pLLVMMCAAction = new QAction(QIcon(":/resource/image/tab/llvm.png"), tr("LLVM MCA"), &menu);
    connect(pLLVMMCAAction, &QAction::triggered, this, &CodeInspectorTabWidget::requestLLVMMCA);
    menu.addAction(pLLVMMCAAction);
    
    menu.exec(mapToGlobal(pos));    
}
