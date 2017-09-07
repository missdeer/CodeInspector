#include "stdafx.h"
#include "configdialog.h"
#include "codeinspector.h"
#include "codeeditor.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->edtCompilerOptions->setClearButtonEnabled(true);

    QSplitter* splitter = new QSplitter(this);
    ui->editorLayout->addWidget(splitter);
    m_codeEditor = new CodeEditor(splitter);
    m_codeEditor->initialize();
    m_codeInspector = new CodeInspector(splitter);
    m_codeInspector->initialize();
    splitter->addWidget(m_codeEditor);
    splitter->addWidget(m_codeInspector);

    connect(&m_backend, &GodboltAgent::compilerListRetrieved, this, &MainWindow::onCompilerListRetrieved);
    connect(&m_backend, &GodboltAgent::compiled, this, &MainWindow::onCompiled);
    connect(ui->cbProgrammingLanguageList, SIGNAL(currentIndexChanged(int)), this, SLOT(onSwitchProgrammingLanguage(int)));
    connect(m_codeEditor, &CodeEditor::contentModified, this, &MainWindow::onSourceCodeEdited);
    m_backend.switchProgrammingLanguage(0);
}

MainWindow::~MainWindow()
{
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

void MainWindow::onSourceCodeEdited()
{
    Q_ASSERT(m_codeEditor);
    CompileInfo ci;
    ci.binary = false;
    ci.commentOnly = true;
    ci.labels = true;
    ci.trim = true;
    ci.directives = true;
    ci.intel = false;
    ci.programmingLanguageIndex = ui->cbProgrammingLanguageList->currentIndex();
    ci.compilerIndex = ui->cbCompilerList->currentIndex();
    ci.source = m_codeEditor->getText(m_codeEditor->textLength() + 1);
    ci.userArguments = ui->edtCompilerOptions->text();
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

