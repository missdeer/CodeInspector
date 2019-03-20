#include "stdafx.h"
#include "codeinspectorapp.h"
#include "codeinspectorpane.h"
#include "compilationtabwidget.h"


CompilationTabWidget::CompilationTabWidget(QWidget *parent)
    : QTabWidget (parent)
{
    setTabPosition(South);
    setTabsClosable(true);
    
    addCodeInspectorPaneTab();
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

void CompilationTabWidget::currentCompilerChanged(const QString &compilerName)
{
    QWidget* w = qobject_cast<QWidget*>(sender());
    for (int i = 0; i < count(); i++)
    {
        if (w == widget(i))
        {
            tabBar()->setTabText(i, compilerName);
            break;
        }
    }
}

void CompilationTabWidget::addCodeInspectorPaneTab()
{
    auto pane = new CodeInspectorPane(this);
    pane->initialize();
    connect(pane, &CodeInspectorPane::currentCompilerChanged, this, &CompilationTabWidget::currentCompilerChanged);
    addTab(pane, tr("Compilation 1"));    
}
