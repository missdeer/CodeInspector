#ifndef GODBOLTAGENT_H
#define GODBOLTAGENT_H

#include <QObject>
#include <QList>
#include <QNetworkAccessManager>
#include "library.h"
#include "compilerinfo.hpp"
#include "asmitem.hpp"
#include "optimizationitem.hpp"

class BackendInterface;

class GodboltAgent : public QObject
{
    Q_OBJECT
public:
    explicit GodboltAgent(QNetworkAccessManager& nam, QObject *parent = nullptr);
    GodboltAgent(const GodboltAgent&) = delete;
    void operator=(const GodboltAgent&) = delete;
    GodboltAgent(GodboltAgent&&) = delete;
    void operator=(GodboltAgent&&) = delete;
    ~GodboltAgent() override = default;

    void initialize(BackendInterface* backend);
    void compile(const CompileInfo& ci);
    
    void setEnableLLVMMCA(bool enabled);
    void setEnableAST(bool enabled);
    void setEnableOptimization(bool enabled);
    void setEnableGCCTreeRTL(bool enabled);
    void setEnablePahole(bool enabled);
    void setEnableClangTidy(bool enabled);
    void setGCCTreeRTLArguments(const QString& pass, bool gccTree, bool rtl);
    
    const QString &getCompileStderr() const;
    const QString &getCompileStdout() const;
    const QString &getAsmContent() const;
    const AsmItemList &getAsmItems() const;
    const QStringList &getGccDumpAllPasses() const;    
    const QString &getCurrentGCCDumpPassOutput() const;    
    const QString &getSelectedGCCDumpPass() const;    
    const QString &getClangTidyStderr() const;    
    const QString &getClangTidyStdout() const;    
    const QString &getLLVMMCAStderr() const;    
    const QString &getLLVMMCAStdout() const;    
    const QString &getPaholeStderr() const;    
    const QString &getPaholeStdout() const;    
    const QString &getASTOutput() const;
    const OptimizationItemList &getOptimizationItems() const;
    
signals:
    void compiled();
    void hasGccDumpOutput();
    void hasLLVMMCAOutput();
    void hasOptimizationOutput();
    void hasPaholeOutput();
    void hasClangTidyOutput();
    void hasASTOutput();
private slots:
    void onCompileRequestFinished();
private:
    enum CompilerOption {
        CO_NONE,
        CO_AST = 0x01,
        CO_LLVMMCA = 0x01 << 1,
        CO_GCCTREERTL = 0x01 << 2,
        CO_OPTIMIZATION = 0x01 << 3,
        CO_PAHOLE = 0x01 << 4,
        CO_CLANGTIDY = 0x01 << 5,
    };
    QNetworkAccessManager& m_nam;
    BackendInterface* m_backend;
    QString m_compileStderr;
    QString m_compileStdout;
    QString m_clangTidyStderr;
    QString m_clangTidyStdout;
    QString m_llvmMCAStderr;
    QString m_llvmMCAStdout;
    QString m_paholeStderr;
    QString m_paholeStdout;
    QString m_asmContent;
    AsmItemList m_asmItems;
    QStringList m_gccDumpAllPasses;
    QString m_currentGCCDumpPassOutput;
    QString m_selectedGCCDumpPass;
    QString m_astOutput;
    OptimizationItemList m_optimizationItems;
    int m_compilerOptions { CO_NONE };
    bool m_gccTreeEnabled { true };
    bool m_rtlEnabled { true };
};

#endif // GODBOLTAGENT_H
