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
    void initialize();
    QString currentLanguageName();
    CodeEditor *codeEditor() const;
    
signals:
    void currentLanguageChanged(QString);
public slots:
    
private slots:
    void updateLanguageList();
    void onCurrentLanguageChanged(const QString &text);
private:
    CodeEditor *m_codeEditor;
    QComboBox *m_languageList;
};

#endif // CODEEDITORPANE_H
