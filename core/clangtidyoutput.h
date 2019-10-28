#ifndef CLANGTIDYOUTPUT_H
#define CLANGTIDYOUTPUT_H

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(QLineEdit)

class ScintillaEdit;
class ScintillaConfig;

class ClangTidyOutput : public QWidget
{
    Q_OBJECT
public:
    explicit ClangTidyOutput(QWidget *parent = nullptr);
    void                  initialize();
    void                  setContent(const QString &content);
    [[nodiscard]] QString getToolOptions();
signals:
    void optionsChanged(const QString &);

private:
    ScintillaEdit *  m_scintillaEdit;
    ScintillaConfig *m_sc;
    QLineEdit *      m_toolOptions;
};

#endif // CLANGTIDYOUTPUT_H
