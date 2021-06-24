#ifndef LDDOUTPUT_H
#define LDDOUTPUT_H

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(QLineEdit)

class ScintillaEdit;
class ScintillaConfig;

class LddOutput : public QWidget
{
    Q_OBJECT
public:
    explicit LddOutput(QWidget *parent = nullptr);
    ~LddOutput();
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

#endif // LDDOUTPUT_H
