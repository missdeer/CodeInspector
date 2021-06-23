#include "wandboxplugin.h"

WandboxPlugin::WandboxPlugin(QObject *parent) : QObject(parent)
{
    
}

bool WandboxPlugin::isCompilerSupported(const QString &compilerId) const
{
    return m_supportedCompilers.contains(compilerId);
}
