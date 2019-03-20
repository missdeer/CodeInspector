#ifndef SESSIONWIDGET_H
#define SESSIONWIDGET_H

#include <QWidget>

class CodeEditorPane;
class CompilationTabWidget;

class SessionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SessionWidget(QWidget *parent = nullptr);
    void initialize();
signals:
    
public slots:
    
private:
    CodeEditorPane* m_codeEditorPane;
    CompilationTabWidget* m_compilationTabWidget;
    
};

#endif // SESSIONWIDGET_H
