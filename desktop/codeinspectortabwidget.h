#ifndef CODEINSPECTORTABWIDGET_H
#define CODEINSPECTORTABWIDGET_H

#include <QTabWidget>
#include "ScintillaEdit.h"
#include "asmitem.hpp"

class CodeInspector;
class LLVMMachineCodeAnalyzerOutput;
class ASTOutput;
class OptimizationOutput;
class GCCTreeRTLOutput;

class CodeInspectorTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit CodeInspectorTabWidget(QWidget *parent = nullptr);
        
    QMap<int, sptr_t> setCodeInspectorAsmItems(const AsmItemList &items, bool binary);
    void setCodeInspectorContent(const QString &content, bool binary);
    void setLLVMMACContent(const QString &content);
    void setASTContent(const QString &content);
    void setOptimizationContent(const QString &content);
    void setGCCTreeRTLContent(const QString &content);
    void setEnableLLVMMCA(bool enabled);    
    void setEnableAST(bool enabled);    
    void setEnableOptimization(bool enabled);    
    void setEnableGCCTreeRTL(bool enabled); 
    void setEnablePahole(bool enabled);
    void setEnableClangTidy(bool enabled);   
signals:
    void requestLLVMMCA();
    void requestAST();
    void requestOptimization();
    void requestGCCTreeRTL();
    void requestPahole();
    void requestClangTidy();
private slots:
    void onCustomContextMenuRequested(const QPoint &pos);
private:
    CodeInspector *m_codeInspector;
    LLVMMachineCodeAnalyzerOutput *m_llvmMCA{nullptr};
    ASTOutput *m_ast{nullptr};
    OptimizationOutput *m_optimization{nullptr};
    GCCTreeRTLOutput *m_gccTreeRTL{nullptr};
    bool m_enableLLVMMCA{true};
    bool m_enableAST{false};
    bool m_enableOptimization{false};
    bool m_enableGCCTreeRTL{false};
    bool m_enablePahole{false};
    bool m_enableClangTidy{false};
};

#endif // CODEINSPECTORTABWIDGET_H
