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

void CodeInspectorTabWidget::setOptimizationContent(const OptimizationItemList &content)
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
        connect(m_gccTreeRTL, &GCCTreeRTLOutput::refresh, this, &CodeInspectorTabWidget::onRefreshGCCDumpOutput);
    }
    Q_ASSERT(m_gccTreeRTL);
    m_gccTreeRTL->setContent(content);
}

void CodeInspectorTabWidget::setGccDumpAllPasses(const QStringList &passes)
{
    Q_ASSERT(m_gccTreeRTL);
    m_gccTreeRTL->setPasses(passes);
}

void CodeInspectorTabWidget::setSelectedGCCDumpPass(const QString &pass)
{
    Q_ASSERT(m_gccTreeRTL);
    m_gccTreeRTL->setCurrentSelectedPass(pass);
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
        QAction* pClangTidyAction = new QAction(QIcon(":/resource/image/tab/clangtidy.png"), tr("Clang Tidy"), &menu);
        connect(pClangTidyAction, &QAction::triggered, this, &CodeInspectorTabWidget::requestClangTidy);
        menu.addAction(pClangTidyAction);
    }
    
    if (m_enablePahole)
    {
        QAction* pPaholeAction = new QAction(QIcon(":/resource/image/tab/pahole.png"), tr("Pahole"), &menu);
        connect(pPaholeAction, &QAction::triggered, this, &CodeInspectorTabWidget::requestPahole);
        menu.addAction(pPaholeAction);
    }
    
    menu.exec(mapToGlobal(pos));    
}

void CodeInspectorTabWidget::onRefreshGCCDumpOutput()
{
    emit refreshGCCDumpOutput(m_gccTreeRTL->getCurrentSelectedPass(), m_gccTreeRTL->isGCCTreeEnabled(), m_gccTreeRTL->isRTLEnabled());
}

void CodeInspectorTabWidget::setEnableGCCTreeRTL(bool enabled)
{
    m_enableGCCTreeRTL = enabled;
    if (!enabled && m_gccTreeRTL)
    {
        for (int i = 0; i < count(); i++)
        {
            if (widget(i) == m_gccTreeRTL)
            {
                removeTab(i);
                delete m_gccTreeRTL;
                m_gccTreeRTL = nullptr;
                break;
            }
        }
    }
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
    if (!enabled && m_optimization)
    {
        for (int i = 0; i < count(); i++)
        {
            if (widget(i) == m_optimization)
            {
                removeTab(i);
                delete m_optimization;
                m_optimization = nullptr;
                break;
            }
        }
    }
}

void CodeInspectorTabWidget::setEnableAST(bool enabled)
{
    m_enableAST = enabled;
    if (!enabled && m_ast)
    {
        for (int i = 0; i < count(); i++)
        {
            if (widget(i) == m_ast)
            {
                removeTab(i);
                delete m_ast;
                m_ast = nullptr;
                break;
            }
        }
    }
}

void CodeInspectorTabWidget::setEnableLLVMMCA(bool enabled)
{
    m_enableLLVMMCA = enabled;
    if (!enabled && m_llvmMCA)
    {
        for (int i = 0; i < count(); i++)
        {
            if (widget(i) == m_llvmMCA)
            {
                removeTab(i);
                delete m_llvmMCA;
                m_llvmMCA = nullptr;
                break;
            }
        }
    }
}
