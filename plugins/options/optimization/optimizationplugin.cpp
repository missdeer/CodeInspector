#include "optimizationplugin.h"

OptimizationPlugin::OptimizationPlugin(QObject *parent) : QObject(parent)
{
    
}

bool OptimizationPlugin::isCurrentCompilerSupported() const
{
    return false;
}

bool OptimizationPlugin::isCurrentSessionEnabled() const
{
    return false;
}

bool OptimizationPlugin::hasResult() const
{
    return false;
}

QWidget *OptimizationPlugin::outputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
