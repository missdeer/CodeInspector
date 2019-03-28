#ifndef GCCTREERTLOUTPUT_H
#define GCCTREERTLOUTPUT_H

#include <QWidget>
#include "scintillaconfig.h"

QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QCheckBox)

class ScintillaEdit;

class GCCTreeRTLOutput : public QWidget
{
    Q_OBJECT
public:
    explicit GCCTreeRTLOutput(QWidget *parent = nullptr);
    void initialize();
    void setContent(const QString &content);
    void setPasses(const QStringList &passes);
    void setCurrentSelectedPass(const QString &pass);
    QString getCurrentSelectedPass();
    bool isGCCTreeEnabled();
    bool isRTLEnabled();
protected:
    QVariant inputMethodQuery(Qt::InputMethodQuery) const override;
    
signals:
    void refresh();
    
private slots:
    void onCurrentTextChanged(const QString &text);
    void onStateChanged(int);
private:
    ScintillaEdit *m_scintillaEdit;
    QComboBox *m_passes;
    QCheckBox *m_gccTree;
    QCheckBox *m_rtl;
    ScintillaConfig *m_sc;
};

#endif // GCCTREERTLOUTPUT_H
