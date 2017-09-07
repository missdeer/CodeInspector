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
    codeEditor = new CodeEditor(splitter);
    codeEditor->initialize();
    codeInspector = new CodeInspector(splitter);
    codeInspector->initialize();
    splitter->addWidget(codeEditor);
    splitter->addWidget(codeInspector);
}

MainWindow::~MainWindow()
{
    delete ui;
}
