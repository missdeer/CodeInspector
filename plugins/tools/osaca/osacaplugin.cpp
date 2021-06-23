#include "osacaplugin.h"

OSACAPlugin::OSACAPlugin(QObject *parent) : QObject(parent)
{
    
}

bool OSACAPlugin::isCompilerSupported(const QString &compilerId) const
{
    return m_supportedCompilers.contains(compilerId);
}

bool OSACAPlugin::isSessionEnabled() const
{
    return false;
}

bool OSACAPlugin::hasResult(const QJsonObject& jsonObj) const
{
    return false;
}

QWidget *OSACAPlugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
