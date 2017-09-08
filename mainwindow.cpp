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
    QPushButton* btnBinary = new QPushButton("Binary", inspectorPanel);
    btnBinary->setFlat(true);
    toolButtonLayout->addWidget(btnBinary);
    QPushButton* btnLabels = new QPushButton("Labels", inspectorPanel);
    btnLabels->setFlat(true);
    toolButtonLayout->addWidget(btnLabels);
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
    connect(ui->cbCompilerList, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [&](){this->onNeedCompile();});
    m_backend.switchProgrammingLanguage(0);
}

MainWindow::~MainWindow()
{
    if (m_timer->isActive())
        m_timer->stop();
    delete m_timer;
    delete ui;
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
    Q_ASSERT(m_codeEditor);
    CompileInfo ci;
    ci.source = m_codeEditor->getText(m_codeEditor->textLength() + 1);
    if (ci.source.isEmpty())
        return;

    ci.userArguments = ui->edtCompilerOptions->text();
    ci.binary = false;
    ci.commentOnly = true;
    ci.labels = true;
    ci.trim = true;
    ci.directives = true;
    ci.intel = false;
    ci.programmingLanguageIndex = ui->cbProgrammingLanguageList->currentIndex();
    ci.compilerIndex = ui->cbCompilerList->currentIndex();
    m_backend.compile(ci);

    m_codeEditor->setSavePoint();
    Q_ASSERT(m_codeInspector);
    m_codeInspector->setContent(tr("<Compiling...>"));
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
    m_backend.switchProgrammingLanguage(index);
    QStringList lexers = {
        "cpp",
        "d",
        "rust",
        "cpp",
        "cpp",
        "cpp",
    };
    Q_ASSERT(m_codeEditor);
    m_codeEditor->setLanguage(lexers[index]);
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

