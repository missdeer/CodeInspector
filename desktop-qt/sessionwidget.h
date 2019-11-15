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

    void open(const QString &fileName);
    void save();
    void save(const QString &fileName);
    bool isSaved();
    bool hasUnsavedModified();
signals:

public slots:

private:
    CodeEditorPane *      m_codeEditorPane;
    CompilationTabWidget *m_compilationTabWidget;
};

#endif // SESSIONWIDGET_H
