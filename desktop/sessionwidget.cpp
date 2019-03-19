#include "stdafx.h"
#include "codeeditorpane.h"
#include "compilationtabwidget.h"
#include "sessionwidget.h"

SessionWidget::SessionWidget(QWidget *parent) : QWidget(parent)
{
    QSplitter* splitter = new QSplitter(this);
    QHBoxLayout* mainLayout = new QHBoxLayout();
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    
    mainLayout->addWidget(splitter);
    
    m_codeEditorPane = new CodeEditorPane(splitter);
    splitter->addWidget(m_codeEditorPane);
    m_compilationTabWidget = new CompilationTabWidget(splitter);
    splitter->addWidget(m_compilationTabWidget);
}
