#include "llvmmcaplugin.h"
#include "llvmmachinecodeanalyzeroutput.h"
#include "pluginbase.h"

LLVMMCAPlugin::LLVMMCAPlugin(QObject *parent) : QObject(parent)
{
    
}

bool LLVMMCAPlugin::isCompilerSupported(const QString &compilerId) const
{
    return m_supportedCompilers.contains(compilerId);
}

bool LLVMMCAPlugin::isSessionEnabled() const
{
    return false;
}

bool LLVMMCAPlugin::hasResult(const QJsonObject &docObj, QString &stdOut, QString &stdErr) const
{
    return PluginBase::hasToolResult(docObj, "llvm-mca", stdOut, stdErr);
}

QWidget *LLVMMCAPlugin::createOutputWidget(QWidget *parent)
{
    return new LLVMMachineCodeAnalyzerOutput(parent);
}
