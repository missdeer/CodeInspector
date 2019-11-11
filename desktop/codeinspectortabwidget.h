#ifndef CODEINSPECTORTABWIDGET_H
#define CODEINSPECTORTABWIDGET_H

#include <QTabWidget>

#include "ScintillaEdit.h"
#include "asmitem.hpp"
#include "llvmiritem.hpp"
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
class LddOutput;
class X86To6502Output;
class LLVMIRView;
class IncludeWhatYouUseOutput;

class CodeInspectorTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit CodeInspectorTabWidget(QWidget *parent = nullptr);

    void setCodeInspectorAsmItems(const AsmItemList &items, bool binary, QMap<int, intptr_t> &markerMap);
    void setCodeInspectorContent(const QString &content, bool binary);
    void setASTContent(const QString &content);
    void setLLVMIRContent(const QString &content);
    void setLLVMIRItems(const LLVMIRItemList &items, QMap<int, intptr_t> &markerMap);
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
    void setLddContent(const QString &content);
    void setIncludeWhatYouUseContent(const QString &content);

    void setEnableAST(bool enabled);
    void setEnableLLVMIR(bool enabled);
    void setEnableOptimization(bool enabled);
    void setEnableGCCTreeRTL(bool enabled);
    void setEnableLLVMMCA(bool enabled);
    void setEnablePahole(bool enabled);
    void setEnableClangTidy(bool enabled);
    void setEnableClangQuery(bool enabled);
    void setEnableReadElf(bool enabled);
    void setEnableX86To6502(bool enabled);
    void setEnableLdd(bool enabled);
    void setEnableIncludeWhatYouUse(bool enabled);

    [[nodiscard]] bool enableAST() const;
    [[nodiscard]] bool enableLLVMIR() const;
    [[nodiscard]] bool enableOptimization() const;
    [[nodiscard]] bool enableGCCTreeRTL() const;
    [[nodiscard]] bool enableLLVMMCA() const;
    [[nodiscard]] bool enablePahole() const;
    [[nodiscard]] bool enableClangTidy() const;
    [[nodiscard]] bool enableClangQuery() const;
    [[nodiscard]] bool enableReadElf() const;
    [[nodiscard]] bool enableX86To6502() const;
    [[nodiscard]] bool enableLdd() const;
    [[nodiscard]] bool enableIncludeWhatYouUse() const;

    [[nodiscard]] QString getLLVMMCAOptions();
    [[nodiscard]] QString getPaholeOptions();
    [[nodiscard]] QString getClangTidyOptions();
    [[nodiscard]] QString getClangQueryOptions();
    [[nodiscard]] QString getReadElfOptions();
    [[nodiscard]] QString getX86To6502Options();
    [[nodiscard]] QString getLddOptions();
    [[nodiscard]] QString getIncludeWhatYouUseOptions();

signals:
    void requestAST();
    void requestLLVMIR();
    void requestOptimization();
    void requestGCCTreeRTL();
    void requestLLVMMCA();
    void requestPahole();
    void requestClangTidy();
    void requestClangQuery();
    void requestReadElf();
    void requestX86To6502();
    void requestLdd();
    void requestIncludeWhatYouUse();

    void refreshGCCDumpOutput(QString, bool, bool);
    void refreshLLVMMCAOptions(const QString &);
    void refreshPaholeOptions(const QString &);
    void refreshClangTidyOptions(const QString &);
    void refreshClangQuery(const QString &);
    void refreshReadElf(const QString &);
    void refreshX86To6502(const QString &);
    void refreshLdd(const QString &);
    void refreshIncludeWhatYouUse(const QString &);
private slots:
    void onCustomContextMenuRequested(const QPoint &pos);
    void onRefreshGCCDumpOutput();
    void onRequestAST();
    void onRequestLLVMIR();
    void onRequestOptimization();
    void onRequestGCCTreeRTL();
    void onRequestLLVMMCA();
    void onRequestPahole();
    void onRequestClangTidy();
    void onRequestClangQuery();
    void onRequestReadElf();
    void onRequestX86To6502();
    void onRequestLdd();
    void onRequestIncludeWhatYouUse();

private:
    CodeInspector *m_codeInspector;

    ASTOutput *                    m_ast {nullptr};
    OptimizationOutput *           m_optimization {nullptr};
    GCCTreeRTLOutput *             m_gccTreeRTL {nullptr};
    PaholeOutput *                 m_pahole {nullptr};
    ClangTidyOutput *              m_clangTidy {nullptr};
    LLVMMachineCodeAnalyzerOutput *m_llvmMCA {nullptr};
    ClangQueryOutput *             m_clangQuery {nullptr};
    ReadElfOutput *                m_readElf {nullptr};
    X86To6502Output *              m_x86To6502 {nullptr};
    LLVMIRView *                   m_llvmIR {nullptr};
    LddOutput *                    m_ldd {nullptr};
    IncludeWhatYouUseOutput *      m_includeWhatYouUse {nullptr};

    bool m_enableAST {false};
    bool m_enableLLVMIR {false};
    bool m_enableOptimization {false};
    bool m_enableGCCTreeRTL {false};
    bool m_enableLLVMMCA {true};
    bool m_enablePahole {false};
    bool m_enableClangTidy {false};
    bool m_enableClangQuery {false};
    bool m_enableReadElf {false};
    bool m_enableX86To6502 {false};
    bool m_enableLdd {false};
    bool m_enableIncludeWhatYouUse {false};
    void removePage(QWidget **w);
};

#endif // CODEINSPECTORTABWIDGET_H
