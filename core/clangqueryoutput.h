#ifndef CLANGQUERYOUTPUT_H
#define CLANGQUERYOUTPUT_H

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(QLineEdit)

class ScintillaEdit;
class ScintillaConfig;

class ClangQueryOutput : public QWidget
{
    Q_OBJECT
public:
    explicit ClangQueryOutput(QWidget *parent = nullptr);
    void initialize();
    void setContent(const QString& content);
    QString getToolOptions();
signals:    
    void optionsChanged(const QString &);
    
private:
    ScintillaEdit *m_scintillaEdit;
    ScintillaConfig *m_sc;
    QLineEdit *m_toolOptions;
};

#endif // CLANGQUERYOUTPUT_H
