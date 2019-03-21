#include "stdafx.h"
#include "settings.h"
#include "codeinspectorapp.h"
#include "codeeditor.h"
#include "outputwindow.h"
#include "codeeditorpane.h"

CodeEditorPane::CodeEditorPane(QWidget *parent) 
    : QWidget(parent)
{    
    auto *mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    
    auto* topBarLayout = new QHBoxLayout();
    topBarLayout->setContentsMargins(0,0,0,0);
    topBarLayout->setSpacing(0);

    m_languageList = new QComboBox(this);
    topBarLayout->addWidget(m_languageList);
    m_languageList->clear();
    
    mainLayout->addLayout(topBarLayout);

    m_codeEditor = new CodeEditor(this);
    m_codeEditor->initialize();

    mainLayout->addWidget(m_codeEditor);
    
    connect(m_languageList, &QComboBox::currentTextChanged, this, &CodeEditorPane::onCurrentLanguageChanged);
    connect(ciApp, &CodeInspectorApp::languageListReady, this, &CodeEditorPane::updateLanguageList);
}

void CodeEditorPane::updateLanguageList()
{
    auto languages = ciApp->getLanguageList();
    for (const auto language : languages)
    {
        m_languageList->addItem(language->name);
    }
    if (m_languageList->count())
        m_languageList->setCurrentIndex(g_settings->defaultLanguageIndex());
}

void CodeEditorPane::onCurrentLanguageChanged(const QString &text)
{
    if (m_languageList->count() > 1)
        g_settings->setDefaultLanguageIndex(m_languageList->currentIndex());
    emit currentLanguageChanged(text);
}

CodeEditor *CodeEditorPane::codeEditor() const
{
    return m_codeEditor;
}

void CodeEditorPane::initialize()
{
    updateLanguageList();
}

QString CodeEditorPane::currentLanguageName()
{
    return m_languageList->currentText();
}
