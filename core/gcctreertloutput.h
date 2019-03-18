#ifndef GCCTREERTLOUTPUT_H
#define GCCTREERTLOUTPUT_H

#include <QObject>
#include "ScintillaEdit.h"
#include "scintillaconfig.h"

class GCCTreeRTLOutput : public ScintillaEdit
{
    Q_OBJECT
public:
    explicit GCCTreeRTLOutput(QWidget *parent = nullptr);
    void initialize();
    void setContent(const QString& content);
private:
    ScintillaConfig m_sc;
};

#endif // GCCTREERTLOUTPUT_H
