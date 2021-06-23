#include "osacaplugin.h"

OSACAPlugin::OSACAPlugin(QObject *parent) : QObject(parent)
{
    
}

bool OSACAPlugin::isCurrentCompilerSupported() const
{
    return false;
}

bool OSACAPlugin::isCurrentSessionEnabled() const
{
    return false;
}

bool OSACAPlugin::hasResult() const
{
    return false;
}

QWidget *OSACAPlugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
