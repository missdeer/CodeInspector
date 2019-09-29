#include "stdafx.h"
#include "sessionwidget.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    addSessionTab();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    auto session = qobject_cast<SessionWidget*>(ui->tabWidget->widget(index));
    if (session->hasUnsavedModified())
    {
        int res = QMessageBox::question(this, tr("Session modified"), tr("Session has been modified, save it now?"), QMessageBox::Ok | QMessageBox::Cancel);
        if (res == QMessageBox::Ok)
        {
            if (session->isSaved())
                session->save();
            else {
                QString fileName = QFileDialog::getSaveFileName(this, tr("Save Session"), QString(), tr("Session Files (*.cis)"));
                if (!fileName.isEmpty())
                    session->save(fileName);
            }
        }
    }
    ui->tabWidget->removeTab(index);
}

int MainWindow::addSessionTab()
{
    auto session = new SessionWidget(this);
    session->initialize();
    return ui->tabWidget->addTab(session, tr("Untitled %1").arg(ui->tabWidget->count() + 1));    
}

void MainWindow::on_actionExit_triggered()
{
    QCoreApplication::quit();
}

void MainWindow::on_actionVisitHomepage_triggered()
{
    QDesktopServices::openUrl(QUrl("https://ci.minidump.info/"));
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("CodeInspector"), tr("Let you know how machine understands your code.<br>"
                                                     "Special thanks to<br>"
                                                     "<strong>Compiler Explorer</strong>: <a href=\"https://github.com/mattgodbolt/compiler-explorer\">https://github.com/mattgodbolt/compiler-explorer</a><br>"
                                                     "<strong>Wandbox</strong>: <a href=\"https://github.com/melpon/wandbox\">https://github.com/melpon/wandbox</a><br>"
                                                     "<strong>Rextester</strong>: <a href=\"https://rextester.com/\">https://rextester.com/</a>"
                                                     ));
}

void MainWindow::on_actionNewSession_triggered()
{
    addSessionTab();
}

void MainWindow::on_actionOpenSession_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Session"),  QString(), tr("Session Files (*.cis)"));
    if (QFile::exists(fileName))
    {
        int index = addSessionTab();
        auto session = qobject_cast<SessionWidget*>(ui->tabWidget->widget(index));        
        session->open(fileName);
    }
}

void MainWindow::on_actionSaveSession_triggered()
{
    auto session = qobject_cast<SessionWidget*>(ui->tabWidget->currentWidget());
    if (session->isSaved())
        session->save();
    else {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Session"), QString(), tr("Session Files (*.cis)"));
        if (!fileName.isEmpty())
            session->save(fileName);
    }
}

void MainWindow::on_actionSaveAsSession_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Session As"), QString(), tr("Session Files (*.cis)"));
    
    auto session = qobject_cast<SessionWidget*>(ui->tabWidget->currentWidget());
    session->save(fileName);
}
