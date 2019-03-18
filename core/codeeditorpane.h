#ifndef CODEEDITORPANE_H
#define CODEEDITORPANE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPushButton;
class QLineEdit;
QT_END_NAMESPACE

class CodeEditor;
class OutputWindow;

class CodeEditorPane : public QWidget
{
    Q_OBJECT
public:
    explicit CodeEditorPane(QWidget *parent = nullptr);
    
signals:
    
public slots:
    
private:
    CodeEditor* m_codeEditor;
    OutputWindow* m_output;
    QLineEdit* m_compilerArguments;
    QPushButton* m_btnToggleOutput;
};

#endif // CODEEDITORPANE_H
