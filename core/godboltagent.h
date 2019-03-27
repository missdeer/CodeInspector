#ifndef GODBOLTAGENT_H
#define GODBOLTAGENT_H

#include <QObject>
#include <QList>
#include <QNetworkAccessManager>
#include "library.h"
#include "compilerinfo.hpp"
#include "asmitem.hpp"

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
    
    const QString &getCompileStderr() const;
    const QString &getCompileStdout() const;
    const QString &getAsmContent() const;
    const AsmItemList &getAsmItems() const;
    const QStringList &getGccDumpAllPasses() const;    
    const QString &getCurrentGCCDumpPassOutput() const;    
    const QString &getSelectedGCCDumpPass() const;    
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
    QString m_asmContent;
    AsmItemList m_asmItems;
    QStringList m_gccDumpAllPasses;
    QString m_currentGCCDumpPassOutput;
    QString m_selectedGCCDumpPass;
    int m_compilerOptions;
};

#endif // GODBOLTAGENT_H
