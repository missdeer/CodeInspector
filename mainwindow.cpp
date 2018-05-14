#include "stdafx.h"
#include <QQmlContext>
#include "codeinspector.h"
#include "codeeditor.h"
#include "outputwindow.h"
#include "qmldialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_timer(new QTimer),
    m_quickAPI(new QuickWidgetAPI(this)),
    m_appUI(new ApplicationUI(this))
{
    ui->setupUi(this);
    ui->edtCompilerOptions->setClearButtonEnabled(true);

    QSplitter* splitter = new QSplitter(this);
#if defined(Q_OS_IOS)
    splitter->setHandleWidth(10);
#elif defined(Q_OS_ANDROID)
    splitter->setBackgroundRole(QPalette::WindowText);
    splitter->setHandleWidth(50);
    ui->btnConfiguration->setIconSize(QSize(120, 120));
#endif
    ui->editorLayout->addWidget(splitter);

    QWidget* editorPanel = new QWidget(splitter);
    QVBoxLayout* editorPanelLayout = new QVBoxLayout();
    editorPanel->setLayout(editorPanelLayout);
    editorPanelLayout->setContentsMargins(0,0,0,0);
    editorPanelLayout->setSpacing(0);

    m_codeEditor = new CodeEditor(editorPanel);
    m_codeEditor->initialize();

    editorPanelLayout->addWidget(m_codeEditor);

    QHBoxLayout* outputBarLayout = new QHBoxLayout();
    outputBarLayout->setContentsMargins(0,0,0,0);
    outputBarLayout->setSpacing(0);

    m_btnToggleOutput = new QPushButton(QIcon(":/resource/image/errmsg.png"), "");
#if defined(Q_OS_ANDROID)
    m_btnToggleOutput->setIconSize(QSize(120, 120));
#else
    m_btnToggleOutput->setIconSize(QSize(32, 32));
#endif
    m_btnToggleOutput->setFlat(true);
    m_btnToggleOutput->setToolTip(tr("Toggle output window"));
    outputBarLayout->addWidget(m_btnToggleOutput);
    outputBarLayout->addStretch(1);

    editorPanelLayout->addLayout(outputBarLayout);

    m_output = new OutputWindow(editorPanel);
    m_output->initialize();
    editorPanelLayout->addWidget(m_output);

    splitter->addWidget(editorPanel);

    QWidget* inspectorPanel = new QWidget(splitter);
    QVBoxLayout* inspectoerPanelLayout = new QVBoxLayout();
    inspectorPanel->setLayout(inspectoerPanelLayout);

    m_codeInspector = new CodeInspector(splitter);
    m_codeInspector->initialize();

    inspectoerPanelLayout->addWidget(m_codeInspector);
    inspectoerPanelLayout->setContentsMargins(0,0,0,0);
    inspectoerPanelLayout->setSpacing(2);

    splitter->addWidget(inspectorPanel);

    connect(m_btnToggleOutput, &QPushButton::clicked, [&]{m_output->setVisible(!m_output->isVisible());});
    connect(m_timer, &QTimer::timeout, this, &MainWindow::onNeedCompile);
    connect(&m_backend, &GodboltAgent::compilerListRetrieved, this, &MainWindow::onCompilerListRetrieved);
    connect(&m_backend, &GodboltAgent::languageListRetrieved, this, &MainWindow::onLanguageListRetrieved);
    connect(&m_backend, &GodboltAgent::compiled, this, &MainWindow::onCompiled);
    connect(ui->cbLanguageList, SIGNAL(currentIndexChanged(QString)), this, SLOT(onSwitchLanguage(const QString&)));
    connect(m_codeEditor, &CodeEditor::contentModified, this, &MainWindow::onDelayCompile);
    connect(ui->edtCompilerOptions, &QLineEdit::textChanged, this, &MainWindow::onDelayCompile);
    connect(ui->cbCompilerList, SIGNAL(currentIndexChanged(QString)), this, SLOT(onSwitchCompiler(const QString&)));

    onLanguageListRetrieved();

    m_btnToggleOutput->setVisible(false);
    m_output->setVisible(false);
}

MainWindow::~MainWindow()
{
    if (m_timer->isActive())
        m_timer->stop();
    delete m_timer;
    delete m_appUI;
    delete m_quickAPI;
    delete ui;
}

void MainWindow::onCompilerListRetrieved()
{
    auto cl = m_backend.getCompilerList(ui->cbLanguageList->currentText());
    ui->cbCompilerList->clear();
    for (auto c : *cl)
    {
        ui->cbCompilerList->addItem(c->name);
    }
}

void MainWindow::onLanguageListRetrieved()
{
    auto ll = m_backend.getLanguageList();
    if (ll.empty())
        return;

    ui->cbLanguageList->clear();
    for (auto l : ll)
    {
        ui->cbLanguageList->addItem(l->name);
    }

    ui->cbLanguageList->setCurrentIndex(0);
}

void MainWindow::onNeedCompile()
{
    if (!m_backend.canCompile(ui->cbLanguageList->currentText(), ui->cbCompilerList->currentText()))
        return;

    Q_ASSERT(m_codeEditor);
    CompileInfo ci;
    ci.source = m_codeEditor->getText(m_codeEditor->textLength() + 1);
    if (ci.source.isEmpty())
        return;

    ci.userArguments = ui->edtCompilerOptions->text();
    ci.binary = m_quickAPI->binary();
    ci.commentOnly = m_quickAPI->commentOnly();
    ci.labels = m_quickAPI->labels();
    ci.trim = m_quickAPI->trim();
    ci.directives = m_quickAPI->directives();
    ci.intel =m_quickAPI->intel();
    ci.demangle = m_quickAPI->demangle();
    ci.language = ui->cbLanguageList->currentText();
    ci.compiler = ui->cbCompilerList->currentText();
    m_backend.compile(ci);

    m_codeEditor->setSavePoint();
    Q_ASSERT(m_codeInspector);
    m_codeInspector->setContent(tr("<Compiling...>"), m_quickAPI->binary());

    storeToCache(ci.language, ci);
    qDebug() << "store:" << ci.compiler;
}

void MainWindow::onCompiled()
{
    auto content = m_backend.getAsmContent();
    Q_ASSERT(m_codeInspector);
    m_codeInspector->setContent(content, m_quickAPI->binary());
    auto asmItems = m_backend.getAsmItems();
    auto markerMap = m_codeInspector->setAsmItems(asmItems, m_quickAPI->binary());
    Q_ASSERT(m_codeEditor);
    m_codeEditor->setMarkerColor(markerMap);

    auto output = m_backend.getCompileOutput();
    if (output.isEmpty())
    {
        if (m_btnToggleOutput->isVisible())
            m_btnToggleOutput->setVisible(false);
        if (m_output->isVisible())
            m_output->setVisible(false);
    }

    if (!output.isEmpty() && !m_btnToggleOutput->isVisible())
        m_btnToggleOutput->setVisible(true);

    m_output->setContent(output);
}

void MainWindow::onSwitchLanguage(const QString& name)
{
    qDebug() << __FUNCTION__ << name;
    m_codeEditor->clearContent();
    m_backend.switchLanguage(name);

    // language name, lexer name
    QMap<QString, QString> lexerMap = {
        { "C++", "cpp" },
        { "Cppx", "cpp"},
        { "Assembly", "asm"},
        { "CUDA", "cpp"},
        { "LLVM IR", "asm"},
        { "D", "d"},
        { "ispc", "cpp"},
        { "Analysis", "asm"},
        { "C", "cpp"},
        { "Rust", "rust"},
        { "Go", "go"},
        { "Pascal", "pascal"},
        { "Haskell", "haskell"},
        { "Swift", "swift"},
    };
    Q_ASSERT(m_codeEditor);
    m_codeEditor->setLanguage(lexerMap[name]);

    CompileInfo ci;
    if (restoreFromCache(name, ci))
    {
        qDebug() << "restore:" << ci.compiler;
        m_codeEditor->setContent(ci.source);
        ui->edtCompilerOptions->setText(ci.userArguments);
        m_quickAPI->setBinary(ci.binary);
        m_quickAPI->setCommentOnly(ci.commentOnly);
        m_quickAPI->setLabels(ci.labels);
        m_quickAPI->setTrim(ci.trim);
        m_quickAPI->setDirectives(ci.directives);
        m_quickAPI->setIntel(ci.intel);
        m_quickAPI->setDemangle(ci.demangle);
        ui->cbCompilerList->setCurrentText(ci.compiler);
        return;
    }

    QString compilerName = m_backend.getDefaultCompilerName(name);
    if (!compilerName.isEmpty())
    {
        qDebug() << "default compiler name:" << compilerName;
        ui->cbCompilerList->setCurrentText(compilerName);
    }
    m_codeEditor->setContent(m_backend.getExample(name));
}

void MainWindow::onSwitchCompiler(const QString& name)
{
    auto cl = m_backend.getCompilerList(ui->cbLanguageList->currentText());
    auto it = std::find_if(cl->begin(), cl->end(),
                           [&name](CompilerPtr c) { return c->name == name;});
    if(cl->end() != it)
    {
        auto compiler = *it;
        m_quickAPI->setBinaryEnabled(compiler->supportsBinary);
        m_quickAPI->setIntelEnabled(compiler->supportsIntel);
        ui->cbCompilerList->setToolTip(compiler->version);

        onDelayCompile();
    }
}

void MainWindow::onDelayCompile()
{
    if (m_timer->isActive())
        m_timer->stop();

    m_timer->setSingleShot(true);
#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
    m_timer->start(1500);
#else
    m_timer->start(750);
#endif
}

void MainWindow::storeToCache(const QString &name, const CompileInfo &ci)
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

bool MainWindow::restoreFromCache(const QString& name, CompileInfo &ci)
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

void MainWindow::on_btnConfiguration_clicked()
{
    QmlDialog dlg(this);
    dlg.setWindowTitle(tr("Configuration"));
    auto context = dlg.context();
    context->setContextProperty("api", m_quickAPI);
    context->setContextProperty("appUI", m_appUI);
    dlg.loadQml(QUrl("qrc:/resource/qml/main.qml"));
    dlg.exec();
}
