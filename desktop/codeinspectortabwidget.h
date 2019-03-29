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

class CodeInspectorTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit CodeInspectorTabWidget(QWidget *parent = nullptr);
        
    QMap<int, sptr_t> setCodeInspectorAsmItems(const AsmItemList &items, bool binary);
    void setCodeInspectorContent(const QString &content, bool binary);
    void setLLVMMACContent(const QString &content);
    void setClangTidyContent(const QString &content);
    void setPaholeContent(const QString &content);
    void setASTContent(const QString &content);
    void setOptimizationContent(const OptimizationItemList &content);
    void setGCCTreeRTLContent(const QString &content);
    void setGccDumpAllPasses(const QStringList &passes);
    void setSelectedGCCDumpPass(const QString &pass);

    void setEnableLLVMMCA(bool enabled);    
    void setEnableAST(bool enabled);    
    void setEnableOptimization(bool enabled);    
    void setEnableGCCTreeRTL(bool enabled); 
    void setEnablePahole(bool enabled);
    void setEnableClangTidy(bool enabled);   
    bool enableLLVMMCA() const;    
    bool enableAST() const;    
    bool enableOptimization() const;    
    bool enableGCCTreeRTL() const;    
    bool enablePahole() const;    
    bool enableClangTidy() const;    
    
    QString getLLVMMCAOptions();
    QString getPaholeOptions();
    QString getClangTidyOptions();
signals:
    void requestLLVMMCA();
    void requestAST();
    void requestOptimization();
    void requestGCCTreeRTL();
    void requestPahole();
    void requestClangTidy();
    void refreshGCCDumpOutput(QString, bool, bool);
    void refreshLLVMMCAOptions(const QString &);
    void refreshPaholeOptions(const QString &);
    void refreshClangTidyOptions(const QString &);
private slots:
    void onCustomContextMenuRequested(const QPoint &pos);
    void onRefreshGCCDumpOutput();
    void onRequestLLVMMCA();
    void onRequestAST();
    void onRequestOptimization();
    void onRequestGCCTreeRTL();
    void onRequestPahole();
    void onRequestClangTidy();
private:
    CodeInspector *m_codeInspector;
    LLVMMachineCodeAnalyzerOutput *m_llvmMCA{nullptr};
    ASTOutput *m_ast{nullptr};
    OptimizationOutput *m_optimization{nullptr};
    GCCTreeRTLOutput *m_gccTreeRTL{nullptr};
    PaholeOutput *m_pahole{nullptr};
    ClangTidyOutput *m_clangTidy{nullptr};
    bool m_enableLLVMMCA{true};
    bool m_enableAST{false};
    bool m_enableOptimization{false};
    bool m_enableGCCTreeRTL{false};
    bool m_enablePahole{false};
    bool m_enableClangTidy{false};
    void removePage(QWidget **w);
};

#endif // CODEINSPECTORTABWIDGET_H
