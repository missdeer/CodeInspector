#include "rextesterplugin.h"

RextesterPlugin::RextesterPlugin(QObject *parent) : QObject(parent)
{
    
}

bool RextesterPlugin::isCompilerSupported(const QString &compilerId) const
{
    return m_supportedCompilers.contains(compilerId);
}
