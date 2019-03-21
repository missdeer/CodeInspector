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
signals:
    void requestLLVMMCA();
    void requestAST();
    void requestOptimization();
    void requestGCCTreeRTL();
private slots:
    void onCustomContextMenuRequested(const QPoint &pos);
private:
    CodeInspector *m_codeInspector;
    LLVMMachineCodeAnalyzerOutput *m_llvmMCA;
    ASTOutput *m_ast;
    OptimizationOutput *m_optimization;
    GCCTreeRTLOutput *m_gccTreeRTL;
    
};

#endif // CODEINSPECTORTABWIDGET_H
