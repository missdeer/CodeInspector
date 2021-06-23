#include "optimizationplugin.h"

OptimizationPlugin::OptimizationPlugin(QObject *parent) : QObject(parent)
{
    
}

bool OptimizationPlugin::isCompilerSupported() const
{
    return false;
}

bool OptimizationPlugin::isSessionEnabled() const
{
    return false;
}

bool OptimizationPlugin::hasResult() const
{
    return false;
}

QWidget *OptimizationPlugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
