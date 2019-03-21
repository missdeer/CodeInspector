#include "stdafx.h"
#include "settings.h"
#include "codeinspectorapp.h"
#include "codeeditor.h"
#include "godboltagent.h"
#include "codeinspectortabwidget.h"
#include "outputwindow.h"
#include "codeinspectorpane.h"

CodeInspectorPane::CodeInspectorPane(CodeEditor *codeEditor, QWidget *parent)
    : QWidget(parent)
    , m_codeEditor(codeEditor)
    , m_backend(new GodboltAgent(ciApp->networkAccessManager(), this))
    , m_timer(new QTimer)
{
    auto *mainLayout = new QVBoxLayout();
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);
    
    auto *topBarLayout = new QHBoxLayout();
    topBarLayout->setContentsMargins(0,0,0,0);
    topBarLayout->setSpacing(1);
    
    m_compilerList = new QComboBox(this);
    topBarLayout->addWidget(m_compilerList);
    
    m_compilerArguments = new QLineEdit(this);
    m_compilerArguments->setPlaceholderText(tr("Build Options"));
    topBarLayout->addWidget(m_compilerArguments);
            
    QMenu* popupMenu = new QMenu(this);
    m_actionBinrary  = new QAction(tr("Binary"), this);
    m_actionBinrary->setIcon(QIcon(":/resource/image/binary.png"));
    m_actionBinrary->setToolTip(tr("Compile to binary and disassemble the output"));
    m_actionBinrary->setCheckable(true);
    m_actionBinrary->setChecked(g_settings->binary());
    connect(m_actionBinrary, &QAction::triggered, this, &CodeInspectorPane::onActionBinaryTriggered);
    popupMenu->addAction(m_actionBinrary);
    
    m_actionLabel  = new QAction(tr("Label"), this);
    m_actionLabel->setIcon(QIcon(":/resource/image/label.png"));
    m_actionLabel->setToolTip(tr("Filter unused labels from the output"));
    m_actionLabel->setCheckable(true);
    m_actionLabel->setChecked(g_settings->label());
    connect(m_actionLabel, &QAction::triggered, this, &CodeInspectorPane::onActionLabelTriggered);
    popupMenu->addAction(m_actionLabel);
    
    m_actionFunctions  = new QAction(tr("Functions"), this);
    m_actionFunctions->setIcon(QIcon(":/resource/image/function.png"));
    m_actionFunctions->setToolTip(tr("Filter functions from other libraries from the output"));
    m_actionFunctions->setCheckable(true);
    m_actionFunctions->setChecked(g_settings->functions());
    connect(m_actionFunctions, &QAction::triggered, this, &CodeInspectorPane::onActionFunctionsTriggered);
    popupMenu->addAction(m_actionFunctions);
    
    m_actionDirectives  = new QAction(tr("Directives"), this);
    m_actionDirectives->setIcon(QIcon(":/resource/image/directives.png"));
    m_actionDirectives->setToolTip(tr("Filter all assembler directives from the output"));
    m_actionDirectives->setCheckable(true);
    m_actionDirectives->setChecked(g_settings->directives());
    connect(m_actionDirectives, &QAction::triggered, this, &CodeInspectorPane::onActionDirectivesTriggered);
    popupMenu->addAction(m_actionDirectives);
    
    m_actionComments  = new QAction(tr("Comments"), this);
    m_actionComments->setIcon(QIcon(":/resource/image/comment.png"));
    m_actionComments->setToolTip(tr("Remove all lines which are only comments from the output"));
    m_actionComments->setCheckable(true);
    m_actionComments->setChecked(g_settings->comments());
    connect(m_actionComments, &QAction::triggered, this, &CodeInspectorPane::onActionCommentsTriggered);
    popupMenu->addAction(m_actionComments);
    
    m_actionTrim  = new QAction(tr("Trim"), this);
    m_actionTrim->setIcon(QIcon(":/resource/image/trim.png"));
    m_actionTrim->setToolTip(tr("Trim intra-line whitespace"));
    m_actionTrim->setCheckable(true);
    m_actionTrim->setChecked(g_settings->trim());
    connect(m_actionTrim, &QAction::triggered, this, &CodeInspectorPane::onActionTrimTriggered);
    popupMenu->addAction(m_actionTrim);
    
    m_actionIntel  = new QAction(tr("Intel"), this);
    m_actionIntel->setIcon(QIcon(":/resource/image/intel.png"));
    m_actionIntel->setToolTip(tr("Output disassembly in Intel syntax"));
    m_actionIntel->setCheckable(true);
    m_actionIntel->setChecked(g_settings->intel());
    connect(m_actionIntel, &QAction::triggered, this, &CodeInspectorPane::onActionIntelTriggered);
    popupMenu->addAction(m_actionIntel);
    
    m_actionDemangle  = new QAction(tr("Demangle"), this);
    m_actionDemangle->setIcon(QIcon(":/resource/image/demangle.png"));
    m_actionDemangle->setToolTip(tr("Demangle output"));
    m_actionDemangle->setCheckable(true);
    m_actionDemangle->setChecked(g_settings->demangle());
    connect(m_actionDemangle, &QAction::triggered, this, &CodeInspectorPane::onActionDemangleTriggered);
    popupMenu->addAction(m_actionDemangle);
    
    m_btnInspectorOptions = new QPushButton(this);
    m_btnInspectorOptions->setIcon(QIcon(":/resource/image/preferences-activities.png"));
    m_btnInspectorOptions->setMenu(popupMenu);
    topBarLayout->addWidget(m_btnInspectorOptions);
    
    topBarLayout->setStretch(0, 1);
    topBarLayout->setStretch(1, 1);
    mainLayout->addLayout(topBarLayout);
    
    m_splitter = new QSplitter(Qt::Vertical, this);
    mainLayout->addWidget(m_splitter);
    
    m_codeInspectorTabWidget = new CodeInspectorTabWidget(this);
    m_splitter->addWidget(m_codeInspectorTabWidget);
    
    QWidget *outputPane = new QWidget(this);
    
    auto* outputBarLayout = new QHBoxLayout();
    outputBarLayout->setContentsMargins(0,0,0,0);
    outputBarLayout->setSpacing(0);
    
    auto* outputPaneLayout = new QVBoxLayout();
    outputPaneLayout->setContentsMargins(0,0,0,0);
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
    
    m_splitter->addWidget(outputPane);
    
    m_splitter->setSizes(QList<int>() << 4096 << 0);
    auto h = m_splitter->handle(1);
    h->setEnabled(false);
    
    m_backend->initialize(ciApp);
    connect(m_btnToggleOutput, &QPushButton::clicked, this, &CodeInspectorPane::onToggleOutput);
    connect(m_compilerList, &QComboBox::currentTextChanged, this, &CodeInspectorPane::onCurrentCompilerChanged);
    connect(m_compilerArguments, &QLineEdit::textChanged, this, &CodeInspectorPane::onCurrentCompilerArgumentsChanged);
    connect(m_timer, &QTimer::timeout, this, &CodeInspectorPane::onNeedCompile);
    connect(m_codeEditor, &CodeEditor::contentModified, this, &CodeInspectorPane::onDelayCompile);
    connect(m_backend, &GodboltAgent::compiled, this, &CodeInspectorPane::onCompiled);
}

void CodeInspectorPane::initialize()
{

}

void CodeInspectorPane::setCompilerList(CompilerListPtr cl)
{
    m_compilerList->clear();
    for (const auto c : *cl)
    {
        m_compilerList->addItem(c->name);
    }
}

void CodeInspectorPane::setCurrentLanguage(const QString &languageName)
{
    m_languageName = languageName;
}

void CodeInspectorPane::onNeedCompile()
{
    if (!ciApp->canCompile(m_languageName, m_compilerList->currentText()))
        return;
    qDebug() << __FUNCTION__;
    
    Q_ASSERT(m_codeEditor);
    CompileInfo ci;
    ci.source = m_codeEditor->getText(m_codeEditor->textLength() + 1);
    if (ci.source.isEmpty())
        return;
    CompilerPtr compiler = ciApp->getCompiler(m_languageName, m_compilerList->currentText());
    QStringList userArguments = {
        m_compilerArguments->text(),
    };
    //        auto libs = m_quickAPI->libs();
    //        if (libs)
    //        {
    //            for (auto lib : *libs)
    //            {
    //                auto versions = lib->getVersions();
    //                for (auto ver : versions)
    //                {
    //                    if (ver->getSelected())
    //                    {
    //                        auto paths = ver->getPath();
    //                        for (auto& path : paths)
    //                            userArguments.append(compiler->includeFlag + path);
    //                    }
    //                }
    //            }
    //        }
    
    ci.userArguments = userArguments.join(" ");
    ci.binary = m_actionBinrary->isChecked();
    ci.commentOnly = m_actionComments->isChecked();
    ci.labels = m_actionLabel->isChecked();
    ci.trim = m_actionTrim->isChecked();
    ci.directives = m_actionDirectives->isChecked();
    ci.intel = m_actionIntel->isChecked();
    ci.demangle = m_actionDemangle->isChecked();
    ci.functions = m_actionFunctions->isChecked();
    ci.language = m_languageName;
    ci.compiler = m_compilerList->currentText();
    m_backend->compile(ci);
    
    m_codeEditor->setSavePoint();
    Q_ASSERT(m_codeInspectorTabWidget);
    m_codeInspectorTabWidget->setCodeInspectorContent(tr("<Compiling...>"), m_actionBinrary->isChecked());
    
    storeToCache(ci.language, ci);
    storeToCache("lastSession", ci);
    qDebug() << "store:" << ci.compiler;
}

void CodeInspectorPane::onCompiled()
{
    qDebug() << __FUNCTION__;
    auto content = m_backend->getAsmContent();
    Q_ASSERT(m_codeInspectorTabWidget);
    m_codeInspectorTabWidget->setCodeInspectorContent(content, m_actionBinrary->isChecked());
    auto asmItems = m_backend->getAsmItems();
    auto markerMap = m_codeInspectorTabWidget->setCodeInspectorAsmItems(asmItems, m_actionBinrary->isChecked());
    Q_ASSERT(m_codeEditor);
    m_codeEditor->setMarkerColor(markerMap);

    auto output = m_backend->getCompileOutput();
    showOutputWindow(!output.isEmpty());    

    m_output->setContent(output);
}

void CodeInspectorPane::onDelayCompile()
{
    qDebug() << __FUNCTION__;
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
    onDelayCompile();
    emit currentCompilerChanged(compilerName);
}

void CodeInspectorPane::storeToCache(const QString &name, const CompileInfo &ci)
{
    QString d = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir dir(d);
    if (!dir.exists())
        dir.mkpath(d);
    QString path = QString("%1/%2").arg(d).arg(name);
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly))
    {
        qDebug() << "open file " << path << " for writting failed";
        return;
    }

    QDataStream stream(&f);
    stream.startTransaction();
    stream.setVersion(QDataStream::Qt_4_5);

    stream << ci.binary
           << ci.commentOnly
           << ci.compiler
           << ci.directives
           << ci.intel
           << ci.labels
           << ci.language
           << ci.source
           << ci.trim
           << ci.demangle
           << ci.userArguments;
    stream.commitTransaction();
    qDebug() << "commit transaction";
    f.close();
}

bool CodeInspectorPane::restoreFromCache(const QString& name, CompileInfo &ci)
{
    QString d = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QString path = QString("%1/%2").arg(d).arg(name);
    if (!QFile::exists(path))
        return false;
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly))
        return false;

    QDataStream stream(&f);
    stream.setVersion(QDataStream::Qt_4_5);

    stream >> ci.binary
           >> ci.commentOnly
           >> ci.compiler
           >> ci.directives
           >> ci.intel
           >> ci.labels
           >> ci.language
           >> ci.source
           >> ci.trim
           >> ci.demangle
           >> ci.userArguments;

    return true;
}

void CodeInspectorPane::showOutputWindow(bool show)
{
    m_splitter->setSizes(QList<int>() << 4096 << (show ? 32 : 0));
    auto h = m_splitter->handle(1);
    h->setEnabled(show);
}
