#include "readelfplugin.h"

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

bool ReadELFPlugin::hasResult(const QJsonObject &docObj) const
{
    return false;
}

QWidget *ReadELFPlugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
