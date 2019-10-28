#ifndef X86TO6502OUTPUT_H
#define X86TO6502OUTPUT_H

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(QLineEdit)

class ScintillaEdit;
class ScintillaConfig;

class X86To6502Output : public QWidget
{
    Q_OBJECT
public:
    explicit X86To6502Output(QWidget *parent = nullptr);
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

#endif // X86TO6502OUTPUT_H
