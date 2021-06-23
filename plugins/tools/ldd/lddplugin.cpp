#include "lddplugin.h"

LDDPlugin::LDDPlugin(QObject *parent) : QObject(parent)
{
    
}

bool LDDPlugin::isCompilerSupported() const
{
    return false;
}

bool LDDPlugin::isSessionEnabled() const
{
    return false;
}

bool LDDPlugin::hasResult() const
{
    return false;
}

QWidget *LDDPlugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
