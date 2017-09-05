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
    sourceEditor = new ScintillaEdit(ui->sourceEditorContainer);
    codeInspector = new ScintillaEdit(ui->inspectorContainer);
}

MainWindow::~MainWindow()
{
    delete ui;
}
