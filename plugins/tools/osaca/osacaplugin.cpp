#include "osacaplugin.h"

OSACAPlugin::OSACAPlugin(QObject *parent) : QObject(parent)
{
    
}

bool OSACAPlugin::isCompilerSupported(int compilerId) const
{
    return false;
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
