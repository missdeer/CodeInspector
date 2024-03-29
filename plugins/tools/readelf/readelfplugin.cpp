#include "readelfplugin.h"
#include "pluginbase.h"
#include "readelfoutput.h"

ReadELFPlugin::ReadELFPlugin(QObject *parent) : QObject(parent)
{
    
}

bool ReadELFPlugin::isCompilerSupported(const QString &compilerId) const
{
    return m_supportedCompilers.contains(compilerId);
}

bool ReadELFPlugin::isSessionEnabled() const
{
    return false;
}

bool ReadELFPlugin::hasResult(const QJsonObject &docObj, QString &stdOut, QString &stdErr) const
{
    return PluginBase::hasToolResult(docObj, "readelf", stdOut, stdErr);
}

QWidget *ReadELFPlugin::createOutputWidget(QWidget *parent)
{
    return new ReadElfOutput(parent);
}
