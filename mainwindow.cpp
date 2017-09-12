#include "stdafx.h"
#include "configdialog.h"
#include "codeinspector.h"
#include "codeeditor.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_timer(new QTimer)
{
    ui->setupUi(this);
    ui->edtCompilerOptions->setClearButtonEnabled(true);

    QSplitter* splitter = new QSplitter(this);
    ui->editorLayout->addWidget(splitter);
    m_codeEditor = new CodeEditor(splitter);
    m_codeEditor->initialize();

    splitter->addWidget(m_codeEditor);

    QWidget* inspectorPanel = new QWidget(splitter);
    QVBoxLayout* inspectoerPanelLayout = new QVBoxLayout(inspectorPanel);
    inspectorPanel->setLayout(inspectoerPanelLayout);

    QHBoxLayout* toolButtonLayout = new QHBoxLayout(inspectorPanel);
    toolButtonLayout->setContentsMargins(0,0,0,0);
    toolButtonLayout->setSpacing(2);

    struct {
        QPushButton*& btn;
        QString icon;
        QString text;
        QString tooltip;
        bool checked;
    } buttons[] = {
    {m_btnBinary, ":/resource/image/binary.png", tr("Binary"), tr("Compile to binary and disassemble the output"), false},
    {m_btnLabels, ":/resource/image/label.png", tr("Labels"), tr("Filter unused labels from the output"), true},
    {m_btnTrim, ":/resource/image/trim.png", tr("Trim"), tr("Trim intra-line whitespace"), true},
    {m_btnDirectives, ":/resource/image/detectives.png", tr("Directives"), tr("Filter all assembler directives from the output"), true},
    {m_btnIntel, ":/resource/image/intel.png", tr("Intel"), tr("Output disassembly in Intel syntax"), true},
    {m_btnCommentOnly, ":/resource/image/comment.png", tr("Comment Only"), tr("Remove all line which are only comments from the output"), true},
    };

    for (const auto & b : buttons)
    {
        b.btn = new QPushButton(QIcon(b.icon), "", inspectorPanel);
        b.btn->setCheckable(true);
        b.btn->setChecked(b.checked);
        b.btn->setIconSize(QSize(32, 32));
#if !defined(Q_OS_IOS) && !defined(Q_OS_ANDROID)
        b.btn->setText(b.text);
        b.btn->setToolTip(b.tooltip);
#endif
        b.btn->setFlat(true);
        connect(b.btn, &QPushButton::clicked, this, &MainWindow::onDelayCompile);
        toolButtonLayout->addWidget(b.btn);
    }

    toolButtonLayout->addStretch();

    m_codeInspector = new CodeInspector(splitter);
    m_codeInspector->initialize();

    inspectoerPanelLayout->addLayout(toolButtonLayout);
    inspectoerPanelLayout->addWidget(m_codeInspector);
    inspectoerPanelLayout->setContentsMargins(0,0,0,0);
    inspectoerPanelLayout->setSpacing(2);

    splitter->addWidget(inspectorPanel);

    connect(m_timer, &QTimer::timeout, this, &MainWindow::onNeedCompile);
    connect(&m_backend, &GodboltAgent::compilerListRetrieved, this, &MainWindow::onCompilerListRetrieved);
    connect(&m_backend, &GodboltAgent::compiled, this, &MainWindow::onCompiled);
    connect(ui->cbProgrammingLanguageList, SIGNAL(currentIndexChanged(int)), this, SLOT(onSwitchProgrammingLanguage(int)));
    connect(m_codeEditor, &CodeEditor::contentModified, this, &MainWindow::onDelayCompile);
    connect(ui->edtCompilerOptions, &QLineEdit::textChanged, this, &MainWindow::onDelayCompile);
    connect(ui->cbCompilerList, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [&](){this->onDelayCompile();});
    m_backend.switchProgrammingLanguage(0);
}

MainWindow::~MainWindow()
{
    if (m_timer->isActive())
        m_timer->stop();
    delete m_timer;
    delete ui;
}

void MainWindow::postInitialize()
{
    onSwitchProgrammingLanguage(0);
}

void MainWindow::onCompilerListRetrieved()
{
    auto cl = m_backend.getCompilerList(ui->cbProgrammingLanguageList->currentIndex());
    ui->cbCompilerList->clear();
    for (const auto & c : cl)
    {
        ui->cbCompilerList->addItem(c.name);
    }
}

void MainWindow::onNeedCompile()
{
    if (!m_backend.canCompile(ui->cbProgrammingLanguageList->currentIndex(), ui->cbCompilerList->currentIndex()))
        return;

    Q_ASSERT(m_codeEditor);
    CompileInfo ci;
    ci.source = m_codeEditor->getText(m_codeEditor->textLength() + 1);
    if (ci.source.isEmpty())
        return;

    ci.userArguments = ui->edtCompilerOptions->text();
    ci.binary = m_btnBinary->isChecked();
    ci.commentOnly = m_btnCommentOnly->isChecked();
    ci.labels = m_btnLabels->isChecked();
    ci.trim = m_btnTrim->isChecked();
    ci.directives = m_btnDirectives->isChecked();
    ci.intel = m_btnIntel->isChecked();
    ci.programmingLanguageIndex = ui->cbProgrammingLanguageList->currentIndex();
    ci.compilerIndex = ui->cbCompilerList->currentIndex();
    m_backend.compile(ci);

    m_codeEditor->setSavePoint();
    Q_ASSERT(m_codeInspector);
    m_codeInspector->setContent(tr("<Compiling...>"));

    m_cache.insert(ci.programmingLanguageIndex, ci);
    qDebug() << "store:" << ci.compilerIndex;
}

void MainWindow::onCompiled()
{
    auto content = m_backend.getAsmContent();
    Q_ASSERT(m_codeInspector);
    m_codeInspector->setContent(content);

    auto output = m_backend.getCompileOutput();
    auto asmItems = m_backend.getAsmItems();
}

void MainWindow::onSwitchProgrammingLanguage(int index)
{
    m_codeEditor->clearContent();
    m_backend.switchProgrammingLanguage(index);
    QStringList lexers = {
        "cpp",
        "d",
        "rust",
        "go",
        "ispc",
        "haskell",
        "swift",
    };
    Q_ASSERT(m_codeEditor);
    m_codeEditor->setLanguage(lexers[index]);

    auto it = m_cache.find(index);
    qDebug() <<  ( m_cache.end() != it);
    if ( m_cache.end() != it)
    {
        const auto& ci = it.value();
        qDebug() << "restore:" << ci.compilerIndex;
        m_codeEditor->setContent(ci.source);
        ui->edtCompilerOptions->setText(ci.userArguments);
        m_btnBinary->setChecked(ci.binary);
        m_btnCommentOnly->setChecked(ci.commentOnly);
        m_btnLabels->setChecked( ci.labels);
        m_btnTrim->setChecked(ci.trim);
        m_btnDirectives->setChecked(ci.directives);
        m_btnIntel->setChecked(ci.intel);
        ui->cbCompilerList->setCurrentIndex(ci.compilerIndex);
        return;
    }

    QStringList examples = {
        ":/resource/example/cpp/square.cpp",
        ":/resource/example/d/square.d",
        ":/resource/example/rust/square.rs",
        ":/resource/example/go/square.go",
        ":/resource/example/ispc/square.ispc",
        ":/resource/example/haskell/sumoverarray.hs",
        ":/resource/example/swift/square.swift",
    };
    QFile f(examples[index]);
    if (!f.open(QIODevice::ReadOnly))
        return;
    m_codeEditor->setContent(f.readAll());
    f.close();
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

