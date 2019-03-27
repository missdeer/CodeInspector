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
{
    setTabPosition(East);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, this, &CodeInspectorTabWidget::onCustomContextMenuRequested);
    
    m_codeInspector->initialize();
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
        m_llvmMCA->initialize();
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
        m_ast->initialize();
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
        m_optimization->initialize();
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
        m_gccTreeRTL->initialize();
        addTab(m_gccTreeRTL, QIcon(":/resource/image/tab/gcc.png"), tr("GCC Tree/RTL Output"));
    }
    Q_ASSERT(m_gccTreeRTL);
    m_gccTreeRTL->setContent(content);
}

void CodeInspectorTabWidget::onCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    
    if (m_enableOptimization)
    {
        QAction* pOptimizationAction = new QAction(QIcon(":/resource/image/tab/optimization.png"), tr("Optimization Output"), &menu);
        connect(pOptimizationAction, &QAction::triggered, this, &CodeInspectorTabWidget::requestOptimization);
        menu.addAction(pOptimizationAction);        
    }
    
    if (m_enableAST)
    {
        QAction* pASTAction = new QAction(QIcon(":/resource/image/tab/ast.png"), tr("AST"), &menu);
        connect(pASTAction, &QAction::triggered, this, &CodeInspectorTabWidget::requestAST);
        menu.addAction(pASTAction);
    }
    
    if (m_enableGCCTreeRTL)
    {
        QAction* pGCCTreeRTLAction = new QAction(QIcon(":/resource/image/tab/gcc.png"), tr("GCC Tree/RTL Output"), &menu);
        connect(pGCCTreeRTLAction, &QAction::triggered, this, &CodeInspectorTabWidget::requestGCCTreeRTL);
        menu.addAction(pGCCTreeRTLAction);
    }
    
    if (m_enableLLVMMCA)
    {
        QAction* pLLVMMCAAction = new QAction(QIcon(":/resource/image/tab/llvm.png"), tr("LLVM MCA"), &menu);
        connect(pLLVMMCAAction, &QAction::triggered, this, &CodeInspectorTabWidget::requestLLVMMCA);
        menu.addAction(pLLVMMCAAction);        
    }
    
    if (m_enableClangTidy)
    {
        QAction* pClangTidyAction = new QAction(tr("Clang Tidy"), &menu);
        connect(pClangTidyAction, &QAction::triggered, this, &CodeInspectorTabWidget::requestClangTidy);
        menu.addAction(pClangTidyAction);
    }
    
    if (m_enablePahole)
    {
        QAction* pPaholeAction = new QAction(tr("Pahole"), &menu);
        connect(pPaholeAction, &QAction::triggered, this, &CodeInspectorTabWidget::requestPahole);
        menu.addAction(pPaholeAction);
    }
    
    menu.exec(mapToGlobal(pos));    
}

void CodeInspectorTabWidget::setEnableGCCTreeRTL(bool enabled)
{
    m_enableGCCTreeRTL = enabled;
}

void CodeInspectorTabWidget::setEnablePahole(bool enabled)
{
    m_enablePahole = enabled;
}

void CodeInspectorTabWidget::setEnableClangTidy(bool enabled)
{
    m_enableClangTidy = enabled;
}

void CodeInspectorTabWidget::setEnableOptimization(bool enabled)
{
    m_enableOptimization = enabled;
}

void CodeInspectorTabWidget::setEnableAST(bool enabled)
{
    m_enableAST = enabled;
}

void CodeInspectorTabWidget::setEnableLLVMMCA(bool enabled)
{
    m_enableLLVMMCA = enabled;
}
