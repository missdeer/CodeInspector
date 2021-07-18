#include <QHBoxLayout>
#include <QSplitter>

#include "sessionwidget.h"
#include "codeeditor.h"
#include "codeeditorpane.h"
#include "codeinspectorapp.h"
#include "compilationtabwidget.h"
#include "settings.h"

SessionWidget::SessionWidget(QWidget *parent) : QWidget(parent)
{
    auto *splitter   = new QSplitter(this);
    auto *mainLayout = new QHBoxLayout();
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    mainLayout->addWidget(splitter);

    m_codeEditorPane = new CodeEditorPane(splitter);
    splitter->addWidget(m_codeEditorPane);
    m_compilationTabWidget = new CompilationTabWidget(m_codeEditorPane->codeEditor(), splitter);
    splitter->addWidget(m_compilationTabWidget);

    QRect rc = parent->geometry();
    splitter->setSizes(QList<int>() << rc.width() / 2 << rc.width() / 2);

    connect(m_codeEditorPane, &CodeEditorPane::currentLanguageChanged, m_compilationTabWidget, &CompilationTabWidget::onLanguageChanged);

    connect(ciApp, &CodeInspectorApp::compilerListReady, [this]() {
        m_compilationTabWidget->onLanguageChanged(m_codeEditorPane->currentLanguageName());
    });
}

void SessionWidget::initialize()
{
    m_codeEditorPane->initialize();
}

void SessionWidget::open(const QString &fileName)
{
    Q_UNUSED(fileName);
}

void SessionWidget::save() {}

void SessionWidget::save(const QString &fileName)
{
    Q_UNUSED(fileName);
}

bool SessionWidget::isSaved()
{
    return false;
}

bool SessionWidget::hasUnsavedModified()
{
    return false;
}
