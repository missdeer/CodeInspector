#include "stdafx.h"
#include "settings.h"
#include "codeinspectorapp.h"
#include "codeeditorpane.h"
#include "compilationtabwidget.h"
#include "sessionwidget.h"

SessionWidget::SessionWidget(QWidget *parent) : QWidget(parent)
{
    auto* splitter = new QSplitter(this);
    auto* mainLayout = new QHBoxLayout();
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    
    mainLayout->addWidget(splitter);
    
    m_codeEditorPane = new CodeEditorPane(splitter);
    splitter->addWidget(m_codeEditorPane);
    m_compilationTabWidget = new CompilationTabWidget(splitter);
    splitter->addWidget(m_compilationTabWidget);

    connect(m_codeEditorPane, &CodeEditorPane::currentLanguageChanged,
            m_compilationTabWidget, &CompilationTabWidget::languageChanged);

    connect(ciApp, &CodeInspectorApp::compilerListReady,
            [=](){m_compilationTabWidget->languageChanged(m_codeEditorPane->currentLanguageName());});
    
//    connect(m_codeEditorPane, &CodeEditorPane::contentModified, [=](){
//        QTimer::singleShot(g_settings->autoRefreshInterval(), this, &SessionWidget::onNeedCompile);
//    });
}

void SessionWidget::initialize()
{
    m_codeEditorPane->initialize();
}

