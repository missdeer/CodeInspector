#ifndef GCCTREERTLOUTPUT_H
#define GCCTREERTLOUTPUT_H

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QCheckBox)

class ScintillaEdit;
class ScintillaConfig;

class GCCTreeRTLOutput : public QWidget
{
    Q_OBJECT
public:
    explicit GCCTreeRTLOutput(QWidget *parent = nullptr);
    ~GCCTreeRTLOutput();
    void                  initialize();
    void                  setContent(const QString &content);
    void                  setPasses(const QStringList &passes);
    void                  setCurrentSelectedPass(const QString &pass);
    [[nodiscard]] QString getCurrentSelectedPass();
    bool                  isGCCTreeEnabled();
    bool                  isRTLEnabled();
signals:
    void refresh();

private slots:
    void onCurrentTextChanged(const QString &text);
    void onStateChanged(int);

private:
    ScintillaEdit *  m_scintillaEdit;
    ScintillaConfig *m_sc;
    QComboBox *      m_passes;
    QCheckBox *      m_gccTree;
    QCheckBox *      m_rtl;
};

#endif // GCCTREERTLOUTPUT_H
