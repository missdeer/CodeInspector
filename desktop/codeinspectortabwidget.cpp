#include "stdafx.h"
#include <QTabBar>
#include "codeinspector.h"
#include "gcctreertloutput.h"
#include "llvmmachinecodeanalyzeroutput.h"
#include "astoutput.h"
#include "optimizationoutput.h"
#include "paholeoutput.h"
#include "clangtidyoutput.h"
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
    Q_ASSERT(m_llvmMCA);
    m_llvmMCA->setContent(content);
}

void CodeInspectorTabWidget::setClangTidyContent(const QString &content)
{
    Q_ASSERT(m_clangTidy);
    m_clangTidy->setContent(content);
}

void CodeInspectorTabWidget::setPaholeContent(const QString &content)
{
    Q_ASSERT(m_pahole);
    m_pahole->setContent(content);
}

void CodeInspectorTabWidget::setClangQueryContent(const QString &content)
{
    
}

void CodeInspectorTabWidget::setReadElfContent(const QString &content)
{
    
}

void CodeInspectorTabWidget::setX86To6502Content(const QString &content)
{
    
}

void CodeInspectorTabWidget::setASTContent(const QString &content)
{
    Q_ASSERT(m_ast);
    m_ast->setContent(content);
}

void CodeInspectorTabWidget::setOptimizationContent(const OptimizationItemList &content)
{
    Q_ASSERT(m_optimization);
    m_optimization->setContent(content);
}

void CodeInspectorTabWidget::setGCCTreeRTLContent(const QString &content)
{
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
    
    QAction* pOptimizationAction = new QAction(QIcon(":/resource/image/tab/optimization.png"), tr("Optimization"), &menu);
    pOptimizationAction->setEnabled(m_enableOptimization);
    connect(pOptimizationAction, &QAction::triggered, this, &CodeInspectorTabWidget::onRequestOptimization);
    menu.addAction(pOptimizationAction);
    
    QAction* pASTAction = new QAction(QIcon(":/resource/image/tab/ast.png"), tr("AST"), &menu);
    pASTAction->setEnabled(m_enableAST);
    connect(pASTAction, &QAction::triggered, this, &CodeInspectorTabWidget::onRequestAST);
    menu.addAction(pASTAction);
    
    QAction* pGCCTreeRTLAction = new QAction(QIcon(":/resource/image/tab/gcc.png"), tr("GCC Tree/RTL"), &menu);
    pGCCTreeRTLAction->setEnabled(m_enableGCCTreeRTL);
    connect(pGCCTreeRTLAction, &QAction::triggered, this, &CodeInspectorTabWidget::onRequestGCCTreeRTL);
    menu.addAction(pGCCTreeRTLAction);
    
    QAction* pLLVMMCAAction = new QAction(QIcon(":/resource/image/tab/llvm.png"), tr("LLVM MCA"), &menu);
    pLLVMMCAAction->setEnabled(m_enableLLVMMCA);
    connect(pLLVMMCAAction, &QAction::triggered, this, &CodeInspectorTabWidget::onRequestLLVMMCA);
    menu.addAction(pLLVMMCAAction);
    
    QAction* pClangTidyAction = new QAction(QIcon(":/resource/image/tab/clangtidy.png"), tr("Clang Tidy"), &menu);
    pClangTidyAction->setEnabled(m_enableClangTidy);
    connect(pClangTidyAction, &QAction::triggered, this, &CodeInspectorTabWidget::onRequestClangTidy);
    menu.addAction(pClangTidyAction);
    
    QAction* pPaholeAction = new QAction(QIcon(":/resource/image/tab/pahole.png"), tr("Pahole"), &menu);
    pPaholeAction->setEnabled(m_enablePahole);
    connect(pPaholeAction, &QAction::triggered, this, &CodeInspectorTabWidget::onRequestPahole);
    menu.addAction(pPaholeAction);
    
    QAction* pClangQueryAction = new QAction(QIcon(":/resource/image/tab/clangquery.png"), tr("ClangQuery"), &menu);
    pClangQueryAction->setEnabled(m_enableClangQuery);
    connect(pClangQueryAction, &QAction::triggered, this, &CodeInspectorTabWidget::onRequestClangQuery);
    menu.addAction(pClangQueryAction);
    
    QAction* pReadElfAction = new QAction(QIcon(":/resource/image/tab/readelf.png"), tr("ReadElf"), &menu);
    pReadElfAction->setEnabled(m_enableReadElf);
    connect(pReadElfAction, &QAction::triggered, this, &CodeInspectorTabWidget::onRequestReadElf);
    menu.addAction(pReadElfAction);
    
    QAction* pX86To6502Action = new QAction(QIcon(":/resource/image/tab/x86to6502.png"), tr("x86 To 6502"), &menu);
    pX86To6502Action->setEnabled(m_enableX86To6502);
    connect(pX86To6502Action, &QAction::triggered, this, &CodeInspectorTabWidget::onRequestX86To6502);
    menu.addAction(pX86To6502Action);
    
    menu.exec(mapToGlobal(pos));    
}

void CodeInspectorTabWidget::onRefreshGCCDumpOutput()
{
    Q_ASSERT(m_gccTreeRTL);
    emit refreshGCCDumpOutput(m_gccTreeRTL->getCurrentSelectedPass(), m_gccTreeRTL->isGCCTreeEnabled(), m_gccTreeRTL->isRTLEnabled());
}

void CodeInspectorTabWidget::onRequestLLVMMCA()
{
    if (!m_llvmMCA)
    {
        m_llvmMCA = new LLVMMachineCodeAnalyzerOutput(this);
        m_llvmMCA->initialize();
        addTab(m_llvmMCA, QIcon(":/resource/image/tab/llvm.png"), tr("LLVM MCA"));
        connect(m_llvmMCA, &LLVMMachineCodeAnalyzerOutput::optionsChanged, this, &CodeInspectorTabWidget::refreshLLVMMCAOptions);
    }
    emit requestLLVMMCA();
}

void CodeInspectorTabWidget::onRequestAST()
{
    if (!m_ast)
    {
        m_ast = new ASTOutput(this);
        m_ast->initialize();
        addTab(m_ast, QIcon(":/resource/image/tab/ast.png"), tr("AST"));
    }
    emit requestAST();
}

void CodeInspectorTabWidget::onRequestOptimization()
{
    if (!m_optimization)
    {
        m_optimization = new OptimizationOutput(this);
        m_optimization->initialize();
        addTab(m_optimization, QIcon(":/resource/image/tab/optimization.png"), tr("Optimization"));
    }
    emit requestOptimization();
}

void CodeInspectorTabWidget::onRequestGCCTreeRTL()
{
    if (!m_gccTreeRTL)
    {
        m_gccTreeRTL = new GCCTreeRTLOutput(this);
        m_gccTreeRTL->initialize();
        addTab(m_gccTreeRTL, QIcon(":/resource/image/tab/gcc.png"), tr("GCC Tree/RTL"));
        connect(m_gccTreeRTL, &GCCTreeRTLOutput::refresh, this, &CodeInspectorTabWidget::onRefreshGCCDumpOutput);
    }
    emit requestGCCTreeRTL();
}

void CodeInspectorTabWidget::onRequestPahole()
{
    if (!m_pahole)
    {
        m_pahole = new PaholeOutput(this);
        m_pahole->initialize();
        addTab(m_pahole, QIcon(":/resource/image/tab/pahole.png"), tr("Pahole"));
        connect(m_pahole, &PaholeOutput::optionsChanged, this, &CodeInspectorTabWidget::refreshPaholeOptions);
    }
    emit requestPahole();
}

void CodeInspectorTabWidget::onRequestClangTidy()
{
    if (!m_clangTidy)
    {
        m_clangTidy = new ClangTidyOutput(this);
        m_clangTidy->initialize();
        addTab(m_clangTidy, QIcon(":/resource/image/tab/clangtidy.png"), tr("Clang Tidy"));
        connect(m_clangTidy, &ClangTidyOutput::optionsChanged, this, &CodeInspectorTabWidget::refreshClangTidyOptions);
    }
    emit requestClangTidy();
}

void CodeInspectorTabWidget::onRequestClangQuery()
{
    emit requestClangQuery();
}

void CodeInspectorTabWidget::onRequestReadElf()
{
    emit requestReadElf();
}

void CodeInspectorTabWidget::onRequestX86To6502()
{
    emit requestX86To6502();
}

void CodeInspectorTabWidget::removePage(QWidget **w)
{
    if (*w)
    {
        for (int i = 0; i < count(); i++)
        {
            if (widget(i) == *w)
            {
                removeTab(i);
                delete *w;
                *w = nullptr;
                break;
            }
        }
    }
}

bool CodeInspectorTabWidget::enableClangTidy() const
{
    return m_enableClangTidy;
}

bool CodeInspectorTabWidget::enableClangQuery() const
{
    return m_enableClangQuery;
}

bool CodeInspectorTabWidget::enableReadElf() const
{
    return m_enableReadElf;
}

bool CodeInspectorTabWidget::enableX86To6502() const
{
    return m_enableX86To6502;
}

QString CodeInspectorTabWidget::getLLVMMCAOptions()
{
    if (m_llvmMCA)
        return m_llvmMCA->getToolOptions();
    return "";
}

QString CodeInspectorTabWidget::getPaholeOptions()
{
    if (m_pahole)
        return m_pahole->getToolOptions();
    return "";
}

QString CodeInspectorTabWidget::getClangTidyOptions()
{
    if (m_clangTidy)
        return m_clangTidy->getToolOptions();
    return "";
}

QString CodeInspectorTabWidget::getClangQueryOptions()
{
    return "";
}

QString CodeInspectorTabWidget::getReadElfOptions()
{
    return "";
}

QString CodeInspectorTabWidget::getX86To6502Options()
{
    return "";
}

bool CodeInspectorTabWidget::enablePahole() const
{
    return m_enablePahole;
}

bool CodeInspectorTabWidget::enableGCCTreeRTL() const
{
    return m_enableGCCTreeRTL;
}

bool CodeInspectorTabWidget::enableOptimization() const
{
    return m_enableOptimization;
}

bool CodeInspectorTabWidget::enableAST() const
{
    return m_enableAST;
}

bool CodeInspectorTabWidget::enableLLVMMCA() const
{
    return m_enableLLVMMCA;
}

void CodeInspectorTabWidget::setEnableGCCTreeRTL(bool enabled)
{
    m_enableGCCTreeRTL = enabled;
    if (!enabled)
        removePage(reinterpret_cast<QWidget **>(&m_gccTreeRTL));
}

void CodeInspectorTabWidget::setEnablePahole(bool enabled)
{
    m_enablePahole = enabled;
    if (!enabled)
        removePage(reinterpret_cast<QWidget **>(&m_pahole));
}

void CodeInspectorTabWidget::setEnableClangTidy(bool enabled)
{
    m_enableClangTidy = enabled;
    if (!enabled)
        removePage(reinterpret_cast<QWidget **>(&m_clangTidy));
}

void CodeInspectorTabWidget::setEnableClangQuery(bool enabled)
{
    m_enableClangQuery = enabled;
}

void CodeInspectorTabWidget::setEnableReadElf(bool enabled)
{
    m_enableReadElf = enabled;
}

void CodeInspectorTabWidget::setEnableX86To6502(bool enabled)
{
    m_enableX86To6502 = enabled;
}

void CodeInspectorTabWidget::setEnableOptimization(bool enabled)
{
    m_enableOptimization = enabled;
    if (!enabled)
        removePage(reinterpret_cast<QWidget **>(&m_optimization));
}

void CodeInspectorTabWidget::setEnableAST(bool enabled)
{
    m_enableAST = enabled;
    if (!enabled)
        removePage(reinterpret_cast<QWidget **>(&m_ast));
}

void CodeInspectorTabWidget::setEnableLLVMMCA(bool enabled)
{
    m_enableLLVMMCA = enabled;
    if (!enabled)
        removePage(reinterpret_cast<QWidget **>(&m_llvmMCA));
}
