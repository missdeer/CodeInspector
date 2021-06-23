#include "readelfplugin.h"

ReadELFPlugin::ReadELFPlugin(QObject *parent) : QObject(parent)
{
    
}

bool ReadELFPlugin::isCurrentCompilerSupported() const
{
    return false;
}

bool ReadELFPlugin::isCurrentSessionEnabled() const
{
    return false;
}

bool ReadELFPlugin::hasResult() const
{
    return false;
}

QWidget *ReadELFPlugin::outputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
