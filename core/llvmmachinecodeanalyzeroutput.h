#ifndef LLVMMACHINECODEANALYZEROUTPUT_H
#define LLVMMACHINECODEANALYZEROUTPUT_H

#include <QObject>
#include "ScintillaEdit.h"
#include "scintillaconfig.h"

class LLVMMachineCodeAnalyzerOutput : public ScintillaEdit
{
    Q_OBJECT
public:
    explicit LLVMMachineCodeAnalyzerOutput(QWidget *parent = nullptr);
    void initialize();
    void setContent(const QString& content);
private:
    ScintillaConfig m_sc;
};

#endif // LLVMMACHINECODEANALYZEROUTPUT_H
