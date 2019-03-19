#include "stdafx.h"
#include "codeinspectorpane.h"
#include "compilationtabwidget.h"

CompilationTabWidget::CompilationTabWidget(QWidget *parent)
    : QTabWidget (parent)
{
    setTabPosition(South);
    addTab(new CodeInspectorPane(this), tr("Compilation 1"));
}
