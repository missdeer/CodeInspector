#include <QAction>
#include <QActionGroup>
#include <QComboBox>
#include <QFile>
#include <QHBoxLayout>
#include <QIcon>
#include <QMenu>
#include <QPushButton>
#include <QVBoxLayout>

#include "codeeditorpane.h"
#include "codeeditor.h"
#include "codeinspectorapp.h"
#include "outputwindow.h"
#include "settings.h"

CodeEditorPane::CodeEditorPane(QWidget *parent) : QWidget(parent), m_codeEditor(new CodeEditor(this)), m_cbLanguageList(new QComboBox(this))
{
    auto *mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    auto *topBarLayout = new QHBoxLayout();
    topBarLayout->setContentsMargins(0, 0, 0, 0);
    topBarLayout->setSpacing(1);

    topBarLayout->addWidget(m_cbLanguageList);
    m_cbLanguageList->clear();
#if defined(Q_OS_WIN)
    m_cbLanguageList->setMaxVisibleItems(100);
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

    m_codeEditor->initialize();

    mainLayout->addWidget(m_codeEditor);

    connect(m_cbLanguageList, &QComboBox::currentTextChanged, this, &CodeEditorPane::onCurrentLanguageChanged);
    connect(ciApp, &CodeInspectorApp::languageListReady, this, &CodeEditorPane::updateLanguageList);
}

void CodeEditorPane::updateLanguageList()
{
    disconnect(m_cbLanguageList, &QComboBox::currentTextChanged, this, &CodeEditorPane::onCurrentLanguageChanged);
    const auto &languages = ciApp->getLanguageList();
    for (int i = 0; i < languages.length(); i++)
    {
        const auto &language = languages[i];
#if defined(LOGS_ENABLED)
        qDebug() << Q_FUNC_INFO << __LINE__ << language->name;
#endif
        int index = m_cbLanguageList->findText(language->name);
        if (index < 0)
        {
            auto iconPath = QStringLiteral(":/resource/image/language/%1.svg").arg(language->id);
            if (!QFile::exists(iconPath))
            {
                iconPath = QStringLiteral(":/resource/image/language/%1-black.svg").arg(language->id);
                if (!QFile::exists(iconPath))
                {
                    iconPath = QStringLiteral(":/resource/image/language/%1.png").arg(language->id);
                    if (!QFile::exists(iconPath))
                    {
                        iconPath = QStringLiteral(":/CodeInspector.png");
                    }
                }
            }
            qDebug() << iconPath << QFile::exists(iconPath) << language->name;
            m_cbLanguageList->insertItem(i, QIcon(iconPath), language->name);
        }
    }
    connect(m_cbLanguageList, &QComboBox::currentTextChanged, this, &CodeEditorPane::onCurrentLanguageChanged);
    Q_ASSERT(m_cbLanguageList->count() > 0);
    m_cbLanguageList->setCurrentIndex(g_settings->defaultLanguageIndex());
}

void CodeEditorPane::onCurrentLanguageChanged(const QString &text)
{
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO << __LINE__ << text;
#endif
    if (m_cbLanguageList->count() > 1)
    {
        g_settings->setDefaultLanguageIndex(m_cbLanguageList->currentIndex());
    }
    emit currentLanguageChanged(text);
    bool show = makeLibariesMenu();
    m_btnLibraries->setVisible(show);
    show = makeExamplesMenu();
    m_btnLoadExample->setVisible(show);

    Q_ASSERT(m_codeEditor);
    m_codeEditor->setLanguage(text);
    if (!show)
    {
        const auto &content = ciApp->getExample(text);
        m_codeEditor->setContent(content);
        return;
    }
    if (m_menuExamples)
    {
        // load an default example
        auto actions = m_menuExamples->actions();
        if (!actions.isEmpty())
        {
            auto *action = actions.at(0);
            emit  action->triggered();
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
    auto  content = ciApp->getExampleContent(m_cbLanguageList->currentText(), name);
    m_codeEditor->setContent(content);
}

void CodeEditorPane::onLibraryVersionTriggered()
{
    auto *action    = qobject_cast<QAction *>(sender());
    auto  libraries = ciApp->getLibraryList(m_cbLanguageList->currentText());
    if (!libraries)
    {
        return;
    }
    auto libraryName = action->data().toString();
    auto iter = std::find_if(libraries->begin(), libraries->end(), [&libraryName](const LibraryPtr &lib) { return lib->getName() == libraryName; });
    if (libraries->end() != iter)
    {
        const auto &lib      = *iter;
        const auto &versions = lib->getVersions();
        QString     version  = action->text();
        for (const auto &ver : versions)
        {
            ver->setSelected(ver->getVersion() == version);
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
    auto libraries = ciApp->getLibraryList(m_cbLanguageList->currentText());
    if (!libraries)
    {
        return false;
    }
    m_menuLibraries = new QMenu(this);
    for (const auto &lib : std::as_const(*libraries))
    {
        const auto &versions = lib->getVersions();
        if (versions.isEmpty())
        {
            continue;
        }
        auto *actionGroup = new QActionGroup(this);
        actionGroup->setExclusive(true);
        QMenu   *menu   = m_menuLibraries->addMenu(lib->getName());
        QAction *action = menu->addAction(" - ");
        connect(action, &QAction::triggered, this, &CodeEditorPane::onLibraryVersionTriggered);
        actionGroup->addAction(action);
        action->setData(lib->getName());
        action->setCheckable(true);
        action->setChecked(true);
        for (const auto &version : versions)
        {
            QAction *action = menu->addAction(version->getVersion());
            action->setData(lib->getName());
            action->setCheckable(true);
            actionGroup->addAction(action);
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
    auto examples = ciApp->getExampleList(m_cbLanguageList->currentText());
    if (examples.isEmpty())
    {
        return false;
    }
    m_menuExamples = new QMenu(this);
    for (const auto &example : examples)
    {
        auto *action = m_menuExamples->addAction(example);
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
    return m_cbLanguageList->currentText();
}
