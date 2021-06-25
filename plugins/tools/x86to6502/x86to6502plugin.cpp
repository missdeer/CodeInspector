#include "x86to6502plugin.h"
#include "pluginbase.h"
#include "x86to6502output.h"

X86To6502Plugin::X86To6502Plugin(QObject *parent) : QObject(parent)
{
    
}

bool X86To6502Plugin::isCompilerSupported(const QString &compilerId) const
{
    return m_supportedCompilers.contains(compilerId);
}

bool X86To6502Plugin::isSessionEnabled() const
{
    return false;
}

bool X86To6502Plugin::hasResult(const QJsonObject &docObj, QString &stdOut, QString &stdErr) const
{
    return PluginBase::hasToolResult(docObj, "x86to6502", stdOut, stdErr);
}

QWidget *X86To6502Plugin::createOutputWidget(QWidget *parent)
{
    return new X86To6502Output(parent);
}
