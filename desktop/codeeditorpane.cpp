#include "stdafx.h"
#include "codeeditor.h"
#include "outputwindow.h"
#include "codeeditorpane.h"

CodeEditorPane::CodeEditorPane(QWidget *parent) 
    : QWidget(parent)
{    
    QVBoxLayout *mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    
    QHBoxLayout* topBarLayout = new QHBoxLayout();
    topBarLayout->setContentsMargins(0,0,0,0);
    topBarLayout->setSpacing(0);

    m_languageList = new QComboBox(this);
    topBarLayout->addWidget(m_languageList);
    
    mainLayout->addLayout(topBarLayout);

    m_codeEditor = new CodeEditor(this);
    m_codeEditor->initialize();

    mainLayout->addWidget(m_codeEditor);
    
    connect(m_codeEditor, &CodeEditor::contentModified, this, &CodeEditorPane::contentModified);
    connect(m_languageList, &QComboBox::currentTextChanged, this, &CodeEditorPane::currentLanguageChanged);
}
