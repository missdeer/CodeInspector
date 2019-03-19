#include "stdafx.h"
#include "sessionwidget.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    auto session = new SessionWidget(this);
    session->initialize();
    ui->tabWidget->addTab(session, tr("Untitled 1"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
