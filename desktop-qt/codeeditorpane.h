#ifndef CODEEDITORPANE_H
#define CODEEDITORPANE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPushButton;
class QComboBox;
class QLineEdit;
class QMenu;
QT_END_NAMESPACE

class CodeEditor;
class OutputWindow;

class CodeEditorPane : public QWidget
{
    Q_OBJECT
public:
    explicit CodeEditorPane(QWidget *parent = nullptr);
    void                      initialize();
    [[nodiscard]] QString     currentLanguageName();
    [[nodiscard]] CodeEditor *codeEditor() const;

signals:
    void currentLanguageChanged(QString);
public slots:

private slots:
    void updateLanguageList();
    void onCurrentLanguageChanged(const QString &text);
    void onShowExampleList();
    void onShowLibraryList();
    void onExampleTriggered();
    void onLibraryVersionTriggered();

private:
    CodeEditor * m_codeEditor;
    QComboBox *  m_cbLanguageList;
    QPushButton *m_btnLibraries;
    QPushButton *m_btnLoadExample;
    QMenu *      m_menuLibraries {nullptr};
    QMenu *      m_menuExamples {nullptr};

    bool makeLibariesMenu();
    bool makeExamplesMenu();
};

#endif // CODEEDITORPANE_H
