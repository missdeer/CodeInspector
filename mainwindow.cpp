#include "stdafx.h"
#include "configdialog.h"
#include "ScintillaEdit.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSplitter* splitter = new QSplitter(this);
    ui->editorLayout->addWidget(splitter);
    codeEditor = new ScintillaEdit(splitter);
    codeInspector = new ScintillaEdit(splitter);
    splitter->addWidget(codeEditor);
    splitter->addWidget(codeInspector);
}

MainWindow::~MainWindow()
{
    delete ui;
}
