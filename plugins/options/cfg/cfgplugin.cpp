#include "cfgplugin.h"

CfgPlugin::CfgPlugin(QObject *parent) : QObject(parent)
{
    
}

bool CfgPlugin::isCompilerSupported() const
{
    return false;
}

bool CfgPlugin::isSessionEnabled() const
{
    return false;
}

bool CfgPlugin::hasResult() const
{
    return false;
}

QWidget *CfgPlugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
