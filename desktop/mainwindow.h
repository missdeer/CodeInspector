#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private slots:
    void on_actionExit_triggered();
    
    void on_actionVisitHomepage_triggered();
    
    void on_actionAbout_triggered();
    
private:
    Ui::MainWindow *ui;
    void addSessionTab();
};

#endif // MAINWINDOW_H