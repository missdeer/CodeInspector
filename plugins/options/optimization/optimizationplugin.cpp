#include "optimizationplugin.h"

OptimizationPlugin::OptimizationPlugin(QObject *parent) : QObject(parent)
{
    
}

bool OptimizationPlugin::isCompilerSupported(int compilerId) const
{
    return false;
}

bool OptimizationPlugin::isSessionEnabled() const
{
    return false;
}

bool OptimizationPlugin::hasResult(const QJsonObject& jsonObj) const
{
    return false;
}

QWidget *OptimizationPlugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
