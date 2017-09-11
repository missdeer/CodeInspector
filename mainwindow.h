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
class QPushButton;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void postInitialize();
private slots:
    void onCompilerListRetrieved();
    void onNeedCompile();
    void onCompiled();
    void onSwitchProgrammingLanguage(int);
    void onDelayCompile();
private:
    Ui::MainWindow *ui;
    QTimer *m_timer;
    CodeEditor* m_codeEditor;
    CodeInspector* m_codeInspector;
    QPushButton* m_btnBinary;
    QPushButton* m_btnLabels;
    QPushButton* m_btnTrim;
    QPushButton* m_btnDirectives;
    QPushButton* m_btnIntel;
    QPushButton* m_btnCommentOnly;
    GodboltAgent m_backend;
    QMap<int, CompileInfo> m_cache;
};

#endif // MAINWINDOW_H
