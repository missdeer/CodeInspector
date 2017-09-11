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
        bool checked;
    } buttons[] = {
    {m_btnBinary, ":/resource/image/binary.png", tr("Binary"), false},
    {m_btnLabels, ":/resource/image/label.png", tr("Labels"), true},
    {m_btnTrim, ":/resource/image/trim.png", tr("Trim"), true},
    {m_btnDirectives, ":/resource/image/detectives.png", tr("Directives"), true},
    {m_btnIntel, ":/resource/image/intel.png", tr("Intel"), true},
    {m_btnCommentOnly, ":/resource/image/comment.png", tr("Comment Only"), true},
    };

    for (const auto & b : buttons)
    {
        b.btn = new QPushButton(QIcon(b.icon), "", inspectorPanel);
        b.btn->setCheckable(true);
        b.btn->setChecked(b.checked);
        b.btn->setIconSize(QSize(32, 32));
        b.btn->setToolTip(b.text);
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
    m_codeEditor->setContent("");
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
        "// Type your code here, or load an example.\nint square(int num) {\n    return num * num;\n}",
        "// Type your code here, or load an example.\nint square(int num) {\n    return num * num;\n}",
        "// Type your code here, or load an example.\npub fn square(num: i32) -> i32 \n{\n    num * num;\n}",
        "// Type your code here, or load an example.\n// Your function name should start with a capital letter.\npackage main\n\nfunc Square(x int) int {\n  return x * x\n}\n\nfunc main() {}",
        "// Type your code here, or load an example.\nuniform int square(uniform int num) {\n    return num * num;\n}",
        "module Example where\n\nsumOverArray :: [Int] -> Int\nsumOverArray (x:xs) = x + sumOverArray xs\nsumOverArray [] =  0",
        "// Type your code here, or load an example.\nfunc square(n: Int) -> Int {\n    return n * n;\n}",
    };
    m_codeEditor->setContent(examples[index]);
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

