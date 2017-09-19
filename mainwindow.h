#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "godboltagent.h"

namespace Ui {
class MainWindow;
}

class CodeEditor;
class CodeInspector;

QT_BEGIN_NAMESPACE
class QToolButton;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void onCompilerListRetrieved();
    void onNeedCompile();
    void onCompiled();
    void onSwitchProgrammingLanguage(int);
    void onSwitchCompiler(int index);
    void onDelayCompile();
private:
    Ui::MainWindow *ui;
    bool m_postInitialized;
    QTimer *m_timer;
    CodeEditor* m_codeEditor;
    CodeInspector* m_codeInspector;
    QToolButton* m_btnBinary;
    QToolButton* m_btnLabels;
    QToolButton* m_btnTrim;
    QToolButton* m_btnDirectives;
    QToolButton* m_btnIntel;
    QToolButton* m_btnCommentOnly;
    GodboltAgent m_backend;

    void storeToCache(int index, const CompileInfo& ci);
    bool restoreFromCache(int index, CompileInfo& ci);
};

#endif // MAINWINDOW_H
