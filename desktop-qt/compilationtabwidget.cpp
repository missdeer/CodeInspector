#include "stdafx.h"

#include "compilationtabwidget.h"

#include "codeinspectorapp.h"
#include "codeinspectorpane.h"

CompilationTabWidget::CompilationTabWidget(CodeEditor *codeEditor, QWidget *parent) : QTabWidget(parent), m_codeEditor(codeEditor)
{
    setTabPosition(South);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, this, &CompilationTabWidget::onCustomContextMenuRequested);
    connect(this, &QTabWidget::tabCloseRequested, this, &CompilationTabWidget::onTabCloseRequested);

    addCodeInspectorPaneTab();
}

void CompilationTabWidget::onLanguageChanged(const QString &languageName)
{
    if (languageName.isEmpty())
        return;
    m_currentLanguageName = languageName;
    auto cl               = ciApp->getCompilerList(languageName);
    for (int i = 0; i < count(); i++)
    {
        auto *w = qobject_cast<CodeInspectorPane *>(widget(i));
        w->setCurrentLanguage(languageName);
        w->setCompilerList(cl);
    }
}

void CompilationTabWidget::onCurrentCompilerChanged(const QString &compilerName)
{
    QWidget *w = qobject_cast<QWidget *>(sender());
    for (int i = 0; i < count(); i++)
    {
        if (w == widget(i))
        {
            tabBar()->setTabText(i, compilerName);
            break;
        }
    }
}

void CompilationTabWidget::onCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);

    QAction *pNewAction = new QAction(QIcon(":/resource/image/tab/inspector.png"), tr("New Compiler"), &menu);
    connect(pNewAction, &QAction::triggered, [this]() { addCodeInspectorPaneTab(); });
    menu.addAction(pNewAction);

    menu.exec(mapToGlobal(pos));
}

void CompilationTabWidget::addCodeInspectorPaneTab()
{
    auto pane = new CodeInspectorPane(m_codeEditor, this);
    pane->initialize();
    connect(pane, &CodeInspectorPane::currentCompilerChanged, this, &CompilationTabWidget::onCurrentCompilerChanged);
    addTab(pane, tr("New Compiler"));
    if (!m_currentLanguageName.isEmpty())
    {
        auto cl = ciApp->getCompilerList(m_currentLanguageName);
        pane->setCurrentLanguage(m_currentLanguageName);
        pane->setCompilerList(cl);
    }
    if (count() > 1)
        setTabsClosable(true);
}

void CompilationTabWidget::onTabCloseRequested(int index)
{
    if (count() > 1)
        removeTab(index);
    if (count() == 1)
        setTabsClosable(false);
}
