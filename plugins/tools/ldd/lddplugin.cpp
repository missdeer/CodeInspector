#include "lddplugin.h"
#include "lddoutput.h"
#include "pluginbase.h"

LDDPlugin::LDDPlugin(QObject *parent) : QObject(parent)
{
    
}

bool LDDPlugin::isCompilerSupported(const QString &compilerId) const
{
    return m_supportedCompilers.contains(compilerId);
}

bool LDDPlugin::isSessionEnabled() const
{
    return false;
}

bool LDDPlugin::hasResult(const QJsonObject &docObj, QString &stdOut, QString &stdErr) const
{
    return PluginBase::hasToolResult(docObj, "ldd", stdOut, stdErr);
}

QWidget *LDDPlugin::createOutputWidget(QWidget *parent)
{
    return new LddOutput(parent);
}
