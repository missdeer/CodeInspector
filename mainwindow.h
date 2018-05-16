#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "godboltagent.h"
#include "quickwidgetapi.h"
#include "applicationui.hpp"

namespace Ui {
class MainWindow;
}

class CodeEditor;
class CodeInspector;
class OutputWindow;

QT_BEGIN_NAMESPACE
class QToolButton;
class QPushButton;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void onCompilerListReady();
    void onLanguageListReady();
    void onConfigurationReady();
    void onNeedCompile();
    void onCompiled();
    void onSwitchLanguage(const QString &name);
    void onSwitchCompiler(const QString &name);
    void onDelayCompile();
    void onLoadSourceCode(const QString &content);
    void on_btnConfiguration_clicked();
private:
    Ui::MainWindow *ui;
    bool m_postInitialized;
    QTimer *m_timer;
    CodeEditor* m_codeEditor;
    CodeInspector* m_codeInspector;
    OutputWindow* m_output;
    QPushButton* m_btnToggleOutput;
    QuickWidgetAPI* m_quickAPI;
    ApplicationUI* m_appUI;
    GodboltAgent m_backend;

    void storeToCache(const QString &name, const CompileInfo& ci);
    bool restoreFromCache(const QString &name, CompileInfo& ci);
    QString getLanguageExampleDirectory(const QString& name);
};

#endif // MAINWINDOW_H
