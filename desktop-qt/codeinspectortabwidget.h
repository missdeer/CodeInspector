#ifndef CODEINSPECTORTABWIDGET_H
#define CODEINSPECTORTABWIDGET_H

#include <QTabWidget>

#include "ScintillaEdit.h"
#include "asmitem.hpp"
#include "llvmiritem.hpp"
#include "optimizationitem.hpp"

class CodeInspector;

class CodeInspectorTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit CodeInspectorTabWidget(QWidget *parent = nullptr);

    void setCodeInspectorAsmItems(const AsmItemList &items, bool binary, QMap<int, intptr_t> &markerMap);
    void setCodeInspectorContent(const QString &content, bool binary);

signals:

private slots:
    void onCustomContextMenuRequested(const QPoint &pos);

private:
    CodeInspector *m_codeInspector;

    void removePage(QWidget **w);
};

#endif // CODEINSPECTORTABWIDGET_H
