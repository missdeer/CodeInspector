#ifndef COMPILATIONTABWIDGET_H
#define COMPILATIONTABWIDGET_H

#include <QTabWidget>

class CompilationTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit CompilationTabWidget(QWidget* parent = nullptr);
public slots:
    void languageChanged(const QString &languageName);
};

#endif // COMPILATIONTABWIDGET_H
