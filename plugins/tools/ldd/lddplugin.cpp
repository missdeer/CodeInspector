#include "lddplugin.h"

LDDPlugin::LDDPlugin(QObject *parent) : QObject(parent)
{
    
}

bool LDDPlugin::isCurrentCompilerSupported() const
{
    return false;
}

bool LDDPlugin::isCurrentSessionEnabled() const
{
    return false;
}

bool LDDPlugin::hasResult() const
{
    return false;
}

QWidget *LDDPlugin::outputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
