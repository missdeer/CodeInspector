#ifndef GODBOLTAGENT_H
#define GODBOLTAGENT_H

#include <QList>
#include <QNetworkAccessManager>
#include <QObject>

#include "asmitem.hpp"
#include "compilerinfo.hpp"
#include "library.h"
#include "llvmiritem.hpp"
#include "optimizationitem.hpp"

class BackendInterface;

class GodboltAgent : public QObject
{
    Q_OBJECT
public:
    explicit GodboltAgent(QNetworkAccessManager &nam, QObject *parent = nullptr);

    GodboltAgent(const GodboltAgent &) = delete;

    void operator=(const GodboltAgent &) = delete;

    GodboltAgent(GodboltAgent &&) = delete;

    void operator=(GodboltAgent &&) = delete;

    ~GodboltAgent() override = default;

    void initialize(BackendInterface *backend);

    void compile(const CompileInfo &ci);

public:
    void setEnableLLVMMCA(bool enabled)
    {
        if (enabled)
            m_compilerOptions |= CO_LLVMMCA;
        else
            m_compilerOptions &= ~CO_LLVMMCA;
    }

    void setEnableAST(bool enabled)
    {
        if (enabled)
            m_compilerOptions |= CO_AST;
        else
            m_compilerOptions &= ~CO_AST;
    }

    void setEnableOptimization(bool enabled)
    {
        if (enabled)
            m_compilerOptions |= CO_OPTIMIZATION;
        else
            m_compilerOptions &= ~CO_OPTIMIZATION;
    }

    void setEnableGCCTreeRTL(bool enabled)
    {
        if (enabled)
            m_compilerOptions |= CO_GCCTREERTL;
        else
            m_compilerOptions &= ~CO_GCCTREERTL;
    }

    void setEnablePahole(bool enabled)
    {
        if (enabled)
            m_compilerOptions |= CO_PAHOLE;
        else
            m_compilerOptions &= ~CO_PAHOLE;
    }

    void setEnableClangTidy(bool enabled)
    {
        if (enabled)
            m_compilerOptions |= CO_CLANGTIDY;
        else
            m_compilerOptions &= ~CO_CLANGTIDY;
    }

    void setEnableLLVMIR(bool enabled)
    {
        if (enabled)
            m_compilerOptions |= CO_LLVMIR;
        else
            m_compilerOptions &= ~CO_LLVMIR;
    }

    void setEnableLdd(bool enabled)
    {
        if (enabled)
            m_compilerOptions |= CO_LDD;
        else
            m_compilerOptions &= ~CO_LDD;
    }

    void setEnableReadElf(bool enabled)
    {
        if (enabled)
            m_compilerOptions |= CO_READELF;
        else
            m_compilerOptions &= ~CO_READELF;
    }

    void setEnabledX86To6502(bool enabled)
    {
        if (enabled)
            m_compilerOptions |= CO_X86TO6502;
        else
            m_compilerOptions &= ~CO_X86TO6502;
    }

    void setEnabledIncludeWhatYouUse(bool enabled)
    {
        if (enabled)
            m_compilerOptions |= CO_INCLUDE_WHAT_YOU_USE;
        else
            m_compilerOptions &= ~CO_INCLUDE_WHAT_YOU_USE;
    }

    void setGCCTreeRTLOptions(const QString &pass, bool gccTree, bool rtl)
    {
        m_selectedGCCDumpPass = pass;
        m_gccTreeEnabled      = gccTree;
        m_rtlEnabled          = rtl;
    }

    void setLLVMMCAOptions(const QString &options)
    {
        m_llvmMCAOptions = options;
    }

    void setPaholeOptions(const QString &options)
    {
        m_paholeOptions = options;
    }

    void setClangTidyOptions(const QString &options)
    {
        m_clangTidyOptions = options;
    }

    void setLddOptions(const QString &options)
    {
        m_lddOptions = options;
    }

    void setReadElfOptions(const QString &options)
    {
        m_readElfOptions = options;
    }

    void setX86To6502Options(const QString &options)
    {
        m_x86To6502Options = options;
    }

    void setIncludeWhatYouUseOptions(const QString &options)
    {
        m_includeWhatYouUseOptions = options;
    }

    [[nodiscard]] const QString &getCompileStderr() const
    {
        return m_compileStderr;
    }

    [[nodiscard]] const QString &getCompileStdout() const
    {
        return m_compileStdout;
    }

    [[nodiscard]] const QString &getAsmContent() const
    {
        return m_asmContent;
    }

    [[nodiscard]] const AsmItemList &getAsmItems() const
    {
        return m_asmItems;
    }

    [[nodiscard]] const QStringList &getGccDumpAllPasses() const
    {
        return m_gccDumpAllPasses;
    }

    [[nodiscard]] const QString &getCurrentGCCDumpPassOutput() const
    {
        return m_currentGCCDumpPassOutput;
    }

    [[nodiscard]] const QString &getSelectedGCCDumpPass() const
    {
        return m_selectedGCCDumpPass;
    }

    [[nodiscard]] const QString &getClangTidyStderr() const
    {
        return m_clangTidyStderr;
    }

    [[nodiscard]] const QString &getClangTidyStdout() const
    {
        return m_clangTidyStdout;
    }

    [[nodiscard]] const QString &getLLVMMCAStderr() const
    {
        return m_llvmMCAStderr;
    }

    [[nodiscard]] const QString &getLLVMMCAStdout() const
    {
        return m_llvmMCAStdout;
    }

    [[nodiscard]] const QString &getPaholeStderr() const
    {
        return m_paholeStderr;
    }

    [[nodiscard]] const QString &getPaholeStdout() const
    {
        return m_paholeStdout;
    }

    [[nodiscard]] const QString &getLddStderr() const
    {
        return m_lddStderr;
    }

    [[nodiscard]] const QString &getLddStdout() const
    {
        return m_lddStdout;
    }

    [[nodiscard]] const QString &getReadElfStderr() const
    {
        return m_readElfStderr;
    }

    [[nodiscard]] const QString &getReadElfStdout() const
    {
        return m_readElfStdout;
    }

    [[nodiscard]] const QString &getX86To6502Stderr() const
    {
        return m_x86To6502Stderr;
    }

    [[nodiscard]] const QString &getX86To6502Stdout() const
    {
        return m_x86To6502Stdout;
    }

    [[nodiscard]] const QString &getIncludeWhatYouUseStderr() const
    {
        return m_includeWhatYouUseStderr;
    }

    [[nodiscard]] const QString &getIncludeWhatYouUseStdout() const
    {
        return m_includeWhatYouUseStdout;
    }

    [[nodiscard]] const QString &getASTOutput() const
    {
        return m_astOutput;
    }

    [[nodiscard]] const OptimizationItemList &getOptimizationItems() const
    {
        return m_optimizationItems;
    }

    [[nodiscard]] const QString &getLLVMIRContent() const
    {
        return m_llvmIRContent;
    }

    [[nodiscard]] const LLVMIRItemList &getLLVMIRItems() const
    {
        return m_llvmIRItems;
    }

signals:

    void compiled();

    void hasGccDumpOutput();

    void hasLLVMMCAOutput();

    void hasOptimizationOutput();

    void hasPaholeOutput();

    void hasClangTidyOutput();

    void hasASTOutput();

    void hasLLVMIROutput();

    void hasLddOutput();

    void hasReadElfOutput();

    void hasX86To6502Output();

    void hasIncludeWhatYouUseOutput();

private slots:

    void onCompileRequestFinished();

private:
    enum CompilerOption
    {
        CO_NONE,
        CO_OPTIMIZATION = 0x01,
        CO_AST          = 0x01 << 1,
        CO_LLVMIR       = 0x01 << 2,
        CO_GCCTREERTL   = 0x01 << 3,
        // CO_GRAPH = 0x01 << 4,

        CO_CLANGTIDY            = 0x01 << 5,
        CO_LLVMMCA              = 0x01 << 6,
        CO_PAHOLE               = 0x01 << 7,
        CO_CLANG_QUERY          = 0x01 << 8,
        CO_READELF              = 0x01 << 9,
        CO_X86TO6502            = 0x01 << 10,
        CO_LDD                  = 0x01 << 11,
        CO_INCLUDE_WHAT_YOU_USE = 0x01 << 12,
    };
    QNetworkAccessManager &m_nam;
    BackendInterface *     m_backend {nullptr};
    QString                m_compileStderr;
    QString                m_compileStdout;
    QString                m_clangTidyStderr;
    QString                m_clangTidyStdout;
    QString                m_clangTidyOptions;
    QString                m_llvmMCAStderr;
    QString                m_llvmMCAStdout;
    QString                m_llvmMCAOptions;
    QString                m_paholeStderr;
    QString                m_paholeStdout;
    QString                m_paholeOptions;
    QString                m_lddStderr;
    QString                m_lddStdout;
    QString                m_lddOptions;
    QString                m_readElfStderr;
    QString                m_readElfStdout;
    QString                m_readElfOptions;
    QString                m_x86To6502Stderr;
    QString                m_x86To6502Stdout;
    QString                m_x86To6502Options;
    QString                m_includeWhatYouUseStderr;
    QString                m_includeWhatYouUseStdout;
    QString                m_includeWhatYouUseOptions;
    QString                m_asmContent;
    AsmItemList            m_asmItems;
    QString                m_llvmIRContent;
    LLVMIRItemList         m_llvmIRItems;
    QStringList            m_gccDumpAllPasses;
    QString                m_currentGCCDumpPassOutput;
    QString                m_selectedGCCDumpPass;
    QString                m_astOutput;
    OptimizationItemList   m_optimizationItems;
    int                    m_compilerOptions {CO_NONE};
    bool                   m_gccTreeEnabled {true};
    bool                   m_rtlEnabled {true};
};

#endif // GODBOLTAGENT_H
