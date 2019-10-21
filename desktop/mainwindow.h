#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private slots:
    void on_tabWidget_tabCloseRequested(int index);

    void on_actionExit_triggered();

    void on_actionVisitHomepage_triggered();

    void on_actionAbout_triggered();

    void on_actionNewSession_triggered();

    void on_actionOpenSession_triggered();

    void on_actionSaveSession_triggered();

    void on_actionSaveAsSession_triggered();

  private:
    Ui::MainWindow *ui;
    int             addSessionTab();
};

#endif // MAINWINDOW_H
