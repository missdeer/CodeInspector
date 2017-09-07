#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "godboltagent.h"

namespace Ui {
class MainWindow;
}

class CodeEditor;
class CodeInspector;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onCompilerListRetrieved();
    void onSourceCodeEdited();
    void onCompiled();
    void onSwitchProgrammingLanguage(int);
private:
    Ui::MainWindow *ui;
    CodeEditor* m_codeEditor;
    CodeInspector* m_codeInspector;
    GodboltAgent m_backend;
};

#endif // MAINWINDOW_H
