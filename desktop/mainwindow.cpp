#include "stdafx.h"
#include "sessionwidget.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->addTab(new SessionWidget(this), tr("Untitled 1"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
