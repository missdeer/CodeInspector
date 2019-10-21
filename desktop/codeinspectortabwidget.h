#ifndef CODEINSPECTORTABWIDGET_H
#define CODEINSPECTORTABWIDGET_H

#include <QTabWidget>
#include "ScintillaEdit.h"
#include "asmitem.hpp"
#include "optimizationitem.hpp"

class CodeInspector;
class LLVMMachineCodeAnalyzerOutput;
class ASTOutput;
class OptimizationOutput;
class GCCTreeRTLOutput;
class PaholeOutput;
class ClangTidyOutput;
class ClangQueryOutput;
class ReadElfOutput;
class X86To6502Output;

class CodeInspectorTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit CodeInspectorTabWidget(QWidget *parent = nullptr);
        
    QMap<int, sptr_t> setCodeInspectorAsmItems(const AsmItemList &items, bool binary);
    void setCodeInspectorContent(const QString &content, bool binary);
    void setASTContent(const QString &content);
    void setIRContent(const QString &content);
    void setOptimizationContent(const OptimizationItemList &content);
    void setGCCTreeRTLContent(const QString &content);
    void setGccDumpAllPasses(const QStringList &passes);
    void setSelectedGCCDumpPass(const QString &pass);
    void setLLVMMACContent(const QString &content);
    void setClangTidyContent(const QString &content);
    void setPaholeContent(const QString &content);
    void setClangQueryContent(const QString &content);
    void setReadElfContent(const QString &content);
    void setX86To6502Content(const QString &content);

    void setEnableAST(bool enabled);
    void setEnableIR(bool enabled);
    void setEnableOptimization(bool enabled);    
    void setEnableGCCTreeRTL(bool enabled); 
    void setEnableLLVMMCA(bool enabled);    
    void setEnablePahole(bool enabled);
    void setEnableClangTidy(bool enabled);   
    void setEnableClangQuery(bool enabled);
    void setEnableReadElf(bool enabled);
    void setEnableX86To6502(bool enabled);
    
    bool enableAST() const;
    bool enableIR() const;
    bool enableOptimization() const;    
    bool enableGCCTreeRTL() const;    
    bool enableLLVMMCA() const;    
    bool enablePahole() const;    
    bool enableClangTidy() const;    
    bool enableClangQuery() const;
    bool enableReadElf() const;
    bool enableX86To6502() const;
    
    QString getLLVMMCAOptions();
    QString getPaholeOptions();
    QString getClangTidyOptions();
    QString getClangQueryOptions();
    QString getReadElfOptions();
    QString getX86To6502Options();
signals:
    void requestAST();
    void requestIR();
    void requestOptimization();
    void requestGCCTreeRTL();
    void requestLLVMMCA();
    void requestPahole();
    void requestClangTidy();
    void requestClangQuery();
    void requestReadElf();
    void requestX86To6502();
    
    void refreshGCCDumpOutput(QString, bool, bool);
    void refreshLLVMMCAOptions(const QString &);
    void refreshPaholeOptions(const QString &);
    void refreshClangTidyOptions(const QString &);
    void refreshClangQuery(const QString &);
    void refreshReadElf(const QString &);
    void refreshX86To6502(const QString &);
private slots:
    void onCustomContextMenuRequested(const QPoint &pos);
    void onRefreshGCCDumpOutput();
    void onRequestAST();
    void onRequestIR();
    void onRequestOptimization();
    void onRequestGCCTreeRTL();
    void onRequestLLVMMCA();
    void onRequestPahole();
    void onRequestClangTidy();
    void onRequestClangQuery();
    void onRequestReadElf();
    void onRequestX86To6502();
private:
    CodeInspector *m_codeInspector;
    
    ASTOutput *m_ast{nullptr};
    OptimizationOutput *m_optimization{nullptr};
    GCCTreeRTLOutput *m_gccTreeRTL{nullptr};
    PaholeOutput *m_pahole{nullptr};
    ClangTidyOutput *m_clangTidy{nullptr};
    LLVMMachineCodeAnalyzerOutput *m_llvmMCA{nullptr};
    ClangQueryOutput *m_clangQuery{nullptr};
    ReadElfOutput *m_readElf{nullptr};
    X86To6502Output *m_x86To6502{nullptr};
    
    bool m_enableAST{false};
    bool m_enableIR{false};
    bool m_enableOptimization{false};
    bool m_enableGCCTreeRTL{false};
    bool m_enableLLVMMCA{true};
    bool m_enablePahole{false};
    bool m_enableClangTidy{false};
    bool m_enableClangQuery{false};
    bool m_enableReadElf{false};
    bool m_enableX86To6502{false};
    void removePage(QWidget **w);
};

#endif // CODEINSPECTORTABWIDGET_H
