#include <QAction>
#include <QMenu>
#include <QTabBar>

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
    {
        return;
    }
    m_currentLanguageName = languageName;
    auto compilerList     = ciApp->getCompilerList(languageName);
    for (int index = 0; index < count(); index++)
    {
        auto *pane = qobject_cast<CodeInspectorPane *>(widget(index));
        pane->setCurrentLanguage(languageName);
        pane->setCompilerList(compilerList);
    }
}

void CompilationTabWidget::onCurrentCompilerChanged(const QString &compilerName)
{
    QWidget *w = qobject_cast<QWidget *>(sender());
    for (int index = 0; index < count(); index++)
    {
        if (w == widget(index))
        {
            tabBar()->setTabText(index, compilerName);
            break;
        }
    }
}

void CompilationTabWidget::onCustomContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);

    auto *pNewAction = new QAction(QIcon(":/resource/image/tab/inspector.png"), tr("New Compiler"), &menu);
    connect(pNewAction, &QAction::triggered, [this]() { addCodeInspectorPaneTab(); });
    menu.addAction(pNewAction);

    menu.exec(mapToGlobal(pos));
}

void CompilationTabWidget::addCodeInspectorPaneTab()
{
    auto *pane = new CodeInspectorPane(m_codeEditor, this);
    pane->initialize();
    connect(pane, &CodeInspectorPane::currentCompilerChanged, this, &CompilationTabWidget::onCurrentCompilerChanged);
    addTab(pane, tr("New Compiler"));
    if (!m_currentLanguageName.isEmpty())
    {
        auto compilerList = ciApp->getCompilerList(m_currentLanguageName);
        pane->setCurrentLanguage(m_currentLanguageName);
        pane->setCompilerList(compilerList);
    }
    if (count() > 1)
    {
        setTabsClosable(true);
    }
}

void CompilationTabWidget::onTabCloseRequested(int index)
{
    if (count() > 1)
    {
        removeTab(index);
    }
    if (count() == 1)
    {
        setTabsClosable(false);
    }
}
