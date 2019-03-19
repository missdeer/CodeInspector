#ifndef CODEEDITORPANE_H
#define CODEEDITORPANE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPushButton;
class QComboBox;
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
    QComboBox* m_languageList;
};

#endif // CODEEDITORPANE_H
