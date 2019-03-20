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

void MainWindow::on_actionExit_triggered()
{
    qApp->quit();
}

void MainWindow::on_actionVisitHomepage_triggered()
{
    QDesktopServices::openUrl(QUrl("https://minidump.info/codeinspector/"));
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("CodeInspector"), tr("Help you to learn how machine runs your code."));
}
