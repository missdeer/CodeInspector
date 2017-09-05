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
    ui->sourceEditorLayout->addWidget(sourceEditor);
    codeInspector = new ScintillaEdit(ui->inspectorContainer);
    ui->inspectorLayout->addWidget(codeInspector);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tbSourceEditor_clicked()
{
    QMessageBox::information(this, "test", "tool button clicked", QMessageBox::Ok);
}

void MainWindow::on_toolButton_clicked()
{
    ConfigDialog dlg(this);
    dlg.exec();
}
