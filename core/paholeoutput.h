#ifndef PAHOLEOUTPUT_H
#define PAHOLEOUTPUT_H

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(QLineEdit)

class ScintillaEdit;
class ScintillaConfig;

class PaholeOutput : public QWidget
{
    Q_OBJECT
public:
    explicit PaholeOutput(QWidget *parent = nullptr);
    ~PaholeOutput();
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

#endif // PAHOLEOUTPUT_H
