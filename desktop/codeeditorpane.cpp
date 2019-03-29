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
    topBarLayout->setSpacing(1);

    m_languageList = new QComboBox(this);
    topBarLayout->addWidget(m_languageList);
    m_languageList->clear();
#if defined(Q_OS_WIN)
    m_languageList->setMaxVisibleItems(100);
#endif
    
    m_btnLibraries = new QPushButton(QIcon(":/resource/image/library.png"), "", this);
    m_btnLibraries->setToolTip(tr("Include libraries"));
    connect(m_btnLibraries, &QPushButton::clicked, this, &CodeEditorPane::onShowLibraryList);
    topBarLayout->addWidget(m_btnLibraries);
    
    m_btnLoadExample = new QPushButton(QIcon(":/resource/image/loadexample.png"), "", this);
    m_btnLoadExample->setToolTip(tr("Load example"));
    connect(m_btnLoadExample, &QPushButton::clicked, this, &CodeEditorPane::onShowExampleList);
    topBarLayout->addWidget(m_btnLoadExample);
    
    topBarLayout->setStretch(0, 1);
    
    mainLayout->addLayout(topBarLayout);

    m_codeEditor = new CodeEditor(this);
    m_codeEditor->initialize();

    mainLayout->addWidget(m_codeEditor);
    
    connect(m_languageList, &QComboBox::currentTextChanged, this, &CodeEditorPane::onCurrentLanguageChanged);
    connect(ciApp, &CodeInspectorApp::languageListReady, this, &CodeEditorPane::updateLanguageList);
}

void CodeEditorPane::updateLanguageList()
{
    m_languageList->clear();
    auto languages = ciApp->getLanguageList();
    for (const auto language : languages)
    {
        m_languageList->addItem(QIcon(QString(":/resource/image/language/%1.png").arg(language->id)), language->name);
    }
    if (m_languageList->count())
        m_languageList->setCurrentIndex(g_settings->defaultLanguageIndex());
}

void CodeEditorPane::onCurrentLanguageChanged(const QString &text)
{
    if (m_languageList->count() > 1)
        g_settings->setDefaultLanguageIndex(m_languageList->currentIndex());
    emit currentLanguageChanged(text);
//    makeLibariesMenu();
    makeExamplesMenu();
}

void CodeEditorPane::onShowExampleList()
{
    if (m_menuExamples)
    {
        QPoint pt = mapToGlobal( QPoint(m_btnLoadExample->x(), m_btnLoadExample->y() + m_btnLoadExample->height()));
        m_menuExamples->exec(pt);
    }
}

void CodeEditorPane::onShowLibraryList()
{
    if (m_menuLibraries)
    {
        m_menuLibraries->exec(QCursor::pos());
    }
}

void CodeEditorPane::onExampleTriggered()
{
    QAction *action = qobject_cast<QAction*>(sender());
    QString name = action->text();
    QString content = ciApp->getExampleContent(m_languageList->currentText(), name);
    m_codeEditor->setContent(content);
}

void CodeEditorPane::makeLibariesMenu()
{
    auto libraries = ciApp->getLibraryList(m_languageList->currentText());
    if (m_menuLibraries)
    {
        delete m_menuLibraries;
        m_menuLibraries = nullptr;
    }
    m_menuLibraries = new QMenu(this);
    for (const auto l : *libraries)
    {
        
    }
}

void CodeEditorPane::makeExamplesMenu()
{
    auto examples = ciApp->getExampleList(m_languageList->currentText());
    if (m_menuExamples)
    {
        delete m_menuExamples;
        m_menuExamples = nullptr;
    }
    m_menuExamples = new QMenu(this);
    for (const auto & e : examples)
    {
        QAction *action = m_menuExamples->addAction(e);
        connect(action, &QAction::triggered, this, &CodeEditorPane::onExampleTriggered);
    }
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
