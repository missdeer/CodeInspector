#ifndef OPTIMIZATIONOUTPUT_H
#define OPTIMIZATIONOUTPUT_H

#include <QObject>
#include "ScintillaEdit.h"
#include "scintillaconfig.h"

class OptimizationOutput : public ScintillaEdit
{
    Q_OBJECT
public:
    explicit OptimizationOutput(QWidget *parent = nullptr);
    void initialize();
    void setContent(const QString& content);
private:
    ScintillaConfig m_sc;
};

#endif // OPTIMIZATIONOUTPUT_H
