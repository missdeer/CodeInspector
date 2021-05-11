#include "stdafx.h"

#include "codeeditorpane.h"

#include "codeeditor.h"
#include "codeinspectorapp.h"
#include "outputwindow.h"
#include "settings.h"

CodeEditorPane::CodeEditorPane(QWidget *parent) : QWidget(parent)
{
    auto *mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    auto *topBarLayout = new QHBoxLayout();
    topBarLayout->setContentsMargins(0, 0, 0, 0);
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
    const auto &languages = ciApp->getLanguageList();
    for (const auto &language : languages)
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
    bool show = makeLibariesMenu();
    m_btnLibraries->setVisible(show);
    show = makeExamplesMenu();
    m_btnLoadExample->setVisible(show);

    if (m_menuExamples)
    {
        // load an default example
        auto actions = m_menuExamples->actions();
        if (!actions.isEmpty())
        {
            auto action = actions.at(0);
            emit action->triggered();
        }
    }
}

void CodeEditorPane::onShowExampleList()
{
    if (!m_menuExamples)
    {
        bool show = makeExamplesMenu();
        m_btnLoadExample->setVisible(show);
    }
    if (m_menuExamples)
    {
        QPoint pt = mapToGlobal(QPoint(m_btnLoadExample->x(), m_btnLoadExample->y() + m_btnLoadExample->height()));
        m_menuExamples->exec(pt);
    }
}

void CodeEditorPane::onShowLibraryList()
{
    if (!m_menuLibraries)
    {
        bool show = makeLibariesMenu();
        m_btnLibraries->setVisible(show);
    }
    if (m_menuLibraries)
    {
        QPoint pt = mapToGlobal(QPoint(m_btnLibraries->x(), m_btnLibraries->y() + m_btnLibraries->height()));
        m_menuLibraries->exec(pt);
    }
}

void CodeEditorPane::onExampleTriggered()
{
    auto *action  = qobject_cast<QAction *>(sender());
    auto  name    = action->text();
    auto  content = ciApp->getExampleContent(m_languageList->currentText(), name);
    m_codeEditor->setContent(content);
}

void CodeEditorPane::onLibraryVersionTriggered()
{
    auto *action    = qobject_cast<QAction *>(sender());
    auto  libraries = ciApp->getLibraryList(m_languageList->currentText());
    if (!libraries)
        return;
    auto libraryName = action->data().toString();
    auto it          = std::find_if(libraries->begin(), libraries->end(), [&libraryName](const LibraryPtr l) { return l->getName() == libraryName; });
    if (libraries->end() != it)
    {
        const auto  l        = *it;
        const auto &versions = l->getVersions();
        QString     version  = action->text();
        for (const auto v : versions)
        {
            v->setSelected(v->getVersion() == version);
        }
    }
}

bool CodeEditorPane::makeLibariesMenu()
{
    if (m_menuLibraries)
    {
        delete m_menuLibraries;
        m_menuLibraries = nullptr;
    }
    auto libraries = ciApp->getLibraryList(m_languageList->currentText());
    if (!libraries)
        return false;
    m_menuLibraries = new QMenu(this);
    for (const auto l : *libraries)
    {
        const auto &versions = l->getVersions();
        if (versions.isEmpty())
            continue;
        auto *ag = new QActionGroup(this);
        ag->setExclusive(true);
        QMenu *  menu   = m_menuLibraries->addMenu(l->getName());
        QAction *action = menu->addAction(" - ");
        connect(action, &QAction::triggered, this, &CodeEditorPane::onLibraryVersionTriggered);
        ag->addAction(action);
        action->setData(l->getName());
        action->setCheckable(true);
        action->setChecked(true);
        for (const auto v : versions)
        {
            QAction *action = menu->addAction(v->getVersion());
            action->setData(l->getName());
            action->setCheckable(true);
            ag->addAction(action);
            connect(action, &QAction::triggered, this, &CodeEditorPane::onLibraryVersionTriggered);
        }
    }
    return true;
}

bool CodeEditorPane::makeExamplesMenu()
{
    if (m_menuExamples)
    {
        delete m_menuExamples;
        m_menuExamples = nullptr;
    }
    auto examples = ciApp->getExampleList(m_languageList->currentText());
    if (examples.isEmpty())
        return false;
    m_menuExamples = new QMenu(this);
    for (const auto &e : examples)
    {
        auto *action = m_menuExamples->addAction(e);
        connect(action, &QAction::triggered, this, &CodeEditorPane::onExampleTriggered);
    }
    return true;
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
