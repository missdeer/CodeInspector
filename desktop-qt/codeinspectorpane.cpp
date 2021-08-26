#include <QComboBox>
#include <QDir>
#include <QFile>
#include <QLineEdit>
#include <QPushButton>
#include <QSplitter>
#include <QStandardItemModel>
#include <QStandardPaths>
#include <QTimer>
#include <QVBoxLayout>

#include "codeinspectorpane.h"
#include "codeeditor.h"
#include "codeinspectorapp.h"
#include "codeinspectortabwidget.h"
#include "comboboxdelegate.h"
#include "godboltagent.h"
#include "outputwindow.h"
#include "settings.h"

enum CompilerUserRole
{
    CURId = Qt::UserRole,
    CURName,
};

CodeInspectorPane::CodeInspectorPane(CodeEditor *codeEditor, QWidget *parent)
    : QWidget(parent)
    , m_codeEditor(codeEditor)
    , m_backend(new GodboltAgent(ciApp->networkAccessManager(), this))
    , m_timer(new QTimer)
{
    auto *mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    auto *topBarLayout = new QHBoxLayout();
    topBarLayout->setContentsMargins(0, 0, 0, 0);
    topBarLayout->setSpacing(1);

    m_compilerList = new QComboBox(this);
    topBarLayout->addWidget(m_compilerList);
#if defined(Q_OS_WIN)
    m_compilerList->setMaxVisibleItems(100);
#endif
    m_compilerList->setItemDelegate(new ComboBoxDelegate);

    m_compilerArguments = new QLineEdit(this);
    m_compilerArguments->setPlaceholderText(tr("Build Options"));
    m_compilerArguments->setClearButtonEnabled(true);
    topBarLayout->addWidget(m_compilerArguments);

    m_btnBinrary = new QPushButton(this);
    m_btnBinrary->setIcon(QIcon(":/resource/image/binary.png"));
    m_btnBinrary->setToolTip(tr("Compile to binary and disassemble the output"));
    m_btnBinrary->setCheckable(true);
    m_btnBinrary->setChecked(g_settings->binary());
    connect(m_btnBinrary, &QPushButton::clicked, this, &CodeInspectorPane::onActionBinaryTriggered);
    topBarLayout->addWidget(m_btnBinrary);

    m_btnLabel = new QPushButton(this);
    m_btnLabel->setIcon(QIcon(":/resource/image/label.png"));
    m_btnLabel->setToolTip(tr("Filter unused labels from the output"));
    m_btnLabel->setCheckable(true);
    m_btnLabel->setChecked(g_settings->label());
    connect(m_btnLabel, &QPushButton::clicked, this, &CodeInspectorPane::onActionLabelTriggered);
    topBarLayout->addWidget(m_btnLabel);

    m_btnFunctions = new QPushButton(this);
    m_btnFunctions->setIcon(QIcon(":/resource/image/function.png"));
    m_btnFunctions->setToolTip(tr("Filter functions from other libraries from the output"));
    m_btnFunctions->setCheckable(true);
    m_btnFunctions->setChecked(g_settings->functions());
    connect(m_btnFunctions, &QPushButton::clicked, this, &CodeInspectorPane::onActionFunctionsTriggered);
    topBarLayout->addWidget(m_btnFunctions);

    m_btnDirectives = new QPushButton(this);
    m_btnDirectives->setIcon(QIcon(":/resource/image/directives.png"));
    m_btnDirectives->setToolTip(tr("Filter all assembler directives from the output"));
    m_btnDirectives->setCheckable(true);
    m_btnDirectives->setChecked(g_settings->directives());
    connect(m_btnDirectives, &QPushButton::clicked, this, &CodeInspectorPane::onActionDirectivesTriggered);
    topBarLayout->addWidget(m_btnDirectives);

    m_btnComments = new QPushButton(this);
    m_btnComments->setIcon(QIcon(":/resource/image/comment.png"));
    m_btnComments->setToolTip(tr("Remove all lines which are only comments from the output"));
    m_btnComments->setCheckable(true);
    m_btnComments->setChecked(g_settings->comments());
    connect(m_btnComments, &QPushButton::clicked, this, &CodeInspectorPane::onActionCommentsTriggered);
    topBarLayout->addWidget(m_btnComments);

    m_btnTrim = new QPushButton(this);
    m_btnTrim->setIcon(QIcon(":/resource/image/trim.png"));
    m_btnTrim->setToolTip(tr("Trim intra-line whitespace"));
    m_btnTrim->setCheckable(true);
    m_btnTrim->setChecked(g_settings->trim());
    connect(m_btnTrim, &QPushButton::clicked, this, &CodeInspectorPane::onActionTrimTriggered);
    topBarLayout->addWidget(m_btnTrim);

    m_btnIntel = new QPushButton(this);
    m_btnIntel->setIcon(QIcon(":/resource/image/intel.png"));
    m_btnIntel->setToolTip(tr("Output disassembly in Intel syntax"));
    m_btnIntel->setCheckable(true);
    m_btnIntel->setChecked(g_settings->intel());
    connect(m_btnIntel, &QPushButton::clicked, this, &CodeInspectorPane::onActionIntelTriggered);
    topBarLayout->addWidget(m_btnIntel);

    m_btnDemangle = new QPushButton(this);
    m_btnDemangle->setIcon(QIcon(":/resource/image/demangle.png"));
    m_btnDemangle->setToolTip(tr("Demangle output"));
    m_btnDemangle->setCheckable(true);
    m_btnDemangle->setChecked(g_settings->demangle());
    connect(m_btnDemangle, &QPushButton::clicked, this, &CodeInspectorPane::onActionDemangleTriggered);
    topBarLayout->addWidget(m_btnDemangle);

    topBarLayout->setStretch(0, 1);
    topBarLayout->setStretch(1, 1);
    mainLayout->addLayout(topBarLayout);

    m_splitter = new QSplitter(Qt::Vertical, this);
    mainLayout->addWidget(m_splitter);

    m_codeInspectorTabWidget = new CodeInspectorTabWidget(this);
    m_splitter->addWidget(m_codeInspectorTabWidget);

    auto *outputPane = new QWidget(this);

    auto *outputBarLayout = new QHBoxLayout();
    outputBarLayout->setContentsMargins(0, 0, 0, 0);
    outputBarLayout->setSpacing(0);

    auto *outputPaneLayout = new QVBoxLayout();
    outputPaneLayout->setContentsMargins(0, 0, 0, 0);
    outputPaneLayout->setSpacing(0);
    outputPane->setLayout(outputPaneLayout);

    m_btnToggleOutput = new QPushButton(QIcon(":/resource/image/errmsg.png"), "");
    m_btnToggleOutput->setIconSize(QSize(32, 32));
    m_btnToggleOutput->setFlat(true);
    m_btnToggleOutput->setToolTip(tr("Toggle output window"));
    outputBarLayout->addWidget(m_btnToggleOutput);
    outputBarLayout->addStretch(1);

    outputPaneLayout->addLayout(outputBarLayout);

    m_output = new OutputWindow(this);
    m_output->initialize();
    outputPaneLayout->addWidget(m_output);
    m_output->setVisible(false);

    m_splitter->addWidget(outputPane);

    m_splitter->setSizes(QList<int>() << 4096 << 0);
    auto h = m_splitter->handle(1);
    h->setEnabled(false);

    m_backend->initialize(ciApp, g_settings->apiBaseURL());
    connect(m_btnToggleOutput, &QPushButton::clicked, this, &CodeInspectorPane::onToggleOutput);
    connect(m_compilerList, &QComboBox::currentTextChanged, this, &CodeInspectorPane::onCurrentCompilerChanged);
    connect(m_compilerArguments, &QLineEdit::textChanged, this, &CodeInspectorPane::onCurrentCompilerArgumentsChanged);
    connect(m_timer, &QTimer::timeout, this, &CodeInspectorPane::onNeedCompile);
    connect(m_codeEditor, &CodeEditor::contentModified, this, &CodeInspectorPane::onDelayCompile);

    connect(m_backend, &GodboltAgent::compiled, this, &CodeInspectorPane::onCompiled);
}

void CodeInspectorPane::initialize() {}

void CodeInspectorPane::setCompilerList(CompilerListPtr cl)
{
    m_compilerList->clear();
    auto *      model = new QStandardItemModel;
    QStringList groupNames;
    for (const auto &c : qAsConst(*cl))
    {
        auto gn = c->groupName.isEmpty() ? c->group.toUpper() : c->groupName.toUpper();
        if (!groupNames.contains(gn))
        {
            groupNames.append(gn);
        }
    }

    std::sort(groupNames.begin(), groupNames.end());

    for (const auto &groupName : groupNames)
    {
        auto *item = new QStandardItem(groupName);
        item->setFlags(item->flags() & ~(Qt::ItemIsEnabled | Qt::ItemIsSelectable));
        item->setData("parent", Qt::AccessibleDescriptionRole);
        QFont font = item->font();
        font.setBold(true);
        item->setFont(font);
        model->appendRow(item);

        for (const auto &compiler : qAsConst(*cl))
        {
            if (compiler->groupName.toUpper() == groupName || compiler->group.toUpper() == groupName)
            {
                auto *item = new QStandardItem(compiler->name + QString(4, QChar(' ')));
                item->setData("child", Qt::AccessibleDescriptionRole);
                item->setData(compiler->id, CURId);
                item->setData(compiler->name, CURName);
                model->appendRow(item);
            }
        }
    }

    m_compilerList->setModel(model);
    if (m_compilerList->count())
    {
        auto c = ciApp->getDefaultCompilerName(m_languageName);
        if (!c.isEmpty())
        {
            int index = m_compilerList->findData(c, Qt::UserRole + 1);
            if (index >= 0)
                m_compilerList->setCurrentIndex(index);
        }
    }
}

void CodeInspectorPane::setCurrentLanguage(const QString &languageName)
{
    m_languageName = languageName;
}

void CodeInspectorPane::onNeedCompile()
{
    if (!ciApp->canCompile(m_languageName, m_compilerList->itemData(m_compilerList->currentIndex(), CURName).toString()))
        return;
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO;
#endif
    Q_ASSERT(m_codeEditor);
    CompileInfo ci;
    ci.source = m_codeEditor->getText(m_codeEditor->textLength() + 1);
    if (ci.source.isEmpty())
        return;
    CompilerPtr compiler      = ciApp->getCompiler(m_languageName, m_compilerList->itemData(m_compilerList->currentIndex(), CURName).toString());
    QStringList userArguments = {
        m_compilerArguments->text(),
    };
    auto libs = ciApp->getLibraryList(m_languageName);
    if (libs)
    {
        for (const auto &lib : qAsConst(*libs))
        {
            auto versions = lib->getVersions();
            for (const auto &ver : versions)
            {
                if (ver->getSelected())
                {
                    auto paths = ver->getPath();
                    for (const auto &path : paths)
                        userArguments.append(compiler->includeFlag + path);
                }
            }
        }
    }

    ci.userArguments = userArguments.join(" ");
    ci.binary        = m_btnBinrary->isChecked();
    ci.commentOnly   = m_btnComments->isChecked();
    ci.labels        = m_btnLabel->isChecked();
    ci.trim          = m_btnTrim->isChecked();
    ci.directives    = m_btnDirectives->isChecked();
    ci.intel         = m_btnIntel->isChecked();
    ci.demangle      = m_btnDemangle->isChecked();
    ci.functions     = m_btnFunctions->isChecked();
    ci.language      = m_languageName;
    ci.compiler      = m_compilerList->itemData(m_compilerList->currentIndex(), CURName).toString();
    m_backend->compile(ci);

    m_codeEditor->setSavePoint();
    Q_ASSERT(m_codeInspectorTabWidget);
    m_codeInspectorTabWidget->setCodeInspectorContent(tr("<Compiling...>"), m_btnBinrary->isChecked());

    storeToCache(ci.language, ci);
    storeToCache("lastSession", ci);
#if defined(LOGS_ENABLED)
    qDebug() << "store:" << ci.compiler;
#endif
}

void CodeInspectorPane::onCompiled()
{
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO;
#endif
    auto content = m_backend->getAsmContent();

    Q_ASSERT(m_codeInspectorTabWidget);
    m_codeInspectorTabWidget->setCodeInspectorContent(content, m_btnBinrary->isChecked());
    auto asmItems = m_backend->getAsmItems();
    m_codeInspectorTabWidget->setCodeInspectorAsmItems(asmItems, m_btnBinrary->isChecked(), m_markerMap);

    Q_ASSERT(m_codeEditor);
    m_codeEditor->setMarkerColor(m_markerMap);

    auto output = m_backend->getCompileStderr();
    showOutputWindow(!output.isEmpty());

    m_output->setContent(output);
}

void CodeInspectorPane::onDelayCompile()
{
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO;
#endif
    if (m_timer->isActive())
        m_timer->stop();

    m_timer->setSingleShot(true);
    m_timer->start(g_settings->autoRefreshInterval());
}

void CodeInspectorPane::onToggleOutput()
{
    bool visible = !m_output->isVisible();
    m_output->setVisible(visible);
    if (!visible)
    {
        m_splitter->setSizes(QList<int>() << 4096 << m_btnToggleOutput->height());
    }
}

void CodeInspectorPane::onActionBinaryTriggered()
{
    onDelayCompile();
}

void CodeInspectorPane::onActionLabelTriggered()
{
    onDelayCompile();
}

void CodeInspectorPane::onActionFunctionsTriggered()
{
    onDelayCompile();
}

void CodeInspectorPane::onActionDirectivesTriggered()
{
    onDelayCompile();
}

void CodeInspectorPane::onActionCommentsTriggered()
{
    onDelayCompile();
}

void CodeInspectorPane::onActionTrimTriggered()
{
    onDelayCompile();
}

void CodeInspectorPane::onActionIntelTriggered()
{
    onDelayCompile();
}

void CodeInspectorPane::onActionDemangleTriggered()
{
    onDelayCompile();
}

void CodeInspectorPane::onCurrentCompilerArgumentsChanged()
{
    onDelayCompile();
    emit currentCompilerArgumentsChanged();
}

void CodeInspectorPane::onCurrentCompilerChanged(const QString &compilerName)
{
    if (compilerName.isEmpty())
        return;
    QString     cn       = compilerName.trimmed();
    CompilerPtr compiler = ciApp->getCompiler(m_languageName, cn);
    if (!compiler)
        return;
    m_compilerList->setToolTip(compiler->version);
    m_btnBinrary->setEnabled(compiler->supportsBinary);
    m_btnIntel->setEnabled(compiler->supportsIntel);
    m_btnDemangle->setEnabled(compiler->supportsDemangle);

    onDelayCompile();
    emit currentCompilerChanged(cn);
}

void CodeInspectorPane::storeToCache(const QString &name, const CompileInfo &ci)
{
    QString d = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir    dir(d);
    if (!dir.exists())
        dir.mkpath(d);
    QString path = QString("%1/%2").arg(d, name);
    QFile   f(path);
    if (!f.open(QIODevice::WriteOnly))
    {
#if defined(LOGS_ENABLED)
        qDebug() << "open file " << path << " for writting failed";
#endif
        return;
    }

    QDataStream stream(&f);
    stream.startTransaction();
    stream.setVersion(QDataStream::Qt_4_5);

    stream << ci.binary << ci.commentOnly << ci.compiler << ci.directives << ci.intel << ci.labels << ci.language << ci.source << ci.trim
           << ci.demangle << ci.userArguments;
    stream.commitTransaction();
#if defined(LOGS_ENABLED)
    qDebug() << "commit transaction";
#endif
    f.close();
}

bool CodeInspectorPane::restoreFromCache(const QString &name, CompileInfo &ci)
{
    QString d    = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QString path = QString("%1/%2").arg(d, name);
    if (!QFile::exists(path))
        return false;
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly))
        return false;

    QDataStream stream(&f);
    stream.setVersion(QDataStream::Qt_4_5);

    stream >> ci.binary >> ci.commentOnly >> ci.compiler >> ci.directives >> ci.intel >> ci.labels >> ci.language >> ci.source >> ci.trim >>
        ci.demangle >> ci.userArguments;

    return true;
}

void CodeInspectorPane::showOutputWindow(bool show)
{
    m_splitter->setSizes(QList<int>() << 4096 << (show ? 32 : 0));
    auto h = m_splitter->handle(1);
    h->setEnabled(show);
    if (!show)
        m_output->setVisible(false);
}
