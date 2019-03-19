#ifndef CODEINSPECTORPANE_H
#define CODEINSPECTORPANE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPushButton;
class QComboBox;
class QLineEdit;
class QSplitter;
QT_END_NAMESPACE

class CodeInspectorTabWidget;
class OutputWindow;
class GodboltAgent;

class CodeInspectorPane : public QWidget
{
    Q_OBJECT
public:
    explicit CodeInspectorPane(QWidget *parent = nullptr);
    
signals:
    void currentCompilerChanged(QString);
    void currentCompilerArgumentsChanged();
public slots:
    
private slots:
    void onToggleOutput();
private:
    QComboBox* m_compilerList;
    QLineEdit* m_compilerArguments;
    QPushButton* m_btnToggleOutput;
    CodeInspectorTabWidget* m_codeInspectorTabWidget;
    OutputWindow* m_output;
    GodboltAgent* m_backend;
    QSplitter *m_splitter;
};

#endif // CODEINSPECTORPANE_H
