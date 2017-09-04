#include "ScintillaEdit.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sourceEditor = new ScintillaEdit(ui->sourceEditorContainer);
    ui->sourceEditorLayout->addWidget(sourceEditor);
    codeInspector = new ScintillaEdit(ui->inspectorContainer);
    ui->inspectorLayout->addWidget(codeInspector);
}

MainWindow::~MainWindow()
{
    delete ui;
}
