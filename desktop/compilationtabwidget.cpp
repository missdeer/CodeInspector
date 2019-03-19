#include "stdafx.h"
#include "codeinspectorapp.h"
#include "codeinspectorpane.h"
#include "compilationtabwidget.h"

CompilationTabWidget::CompilationTabWidget(QWidget *parent)
    : QTabWidget (parent)
{
    setTabPosition(South);
    auto pane = new CodeInspectorPane(this);
    pane->initialize();
    addTab(pane, tr("Compilation 1"));
}

void CompilationTabWidget::languageChanged(const QString &languageName)
{
    auto cl = ciApp->getCompilerList(languageName);
    for (int i = 0; i < count(); i++)
    {
        auto *w = qobject_cast<CodeInspectorPane*>(widget(i));
        w->setCompilerList(cl);
    }
}
