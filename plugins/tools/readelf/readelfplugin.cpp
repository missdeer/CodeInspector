#include "readelfplugin.h"

ReadELFPlugin::ReadELFPlugin(QObject *parent) : QObject(parent)
{
    
}

bool ReadELFPlugin::isCompilerSupported(int compilerId) const
{
    return false;
}

bool ReadELFPlugin::isSessionEnabled() const
{
    return false;
}

bool ReadELFPlugin::hasResult(const QJsonObject& jsonObj) const
{
    return false;
}

QWidget *ReadELFPlugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
