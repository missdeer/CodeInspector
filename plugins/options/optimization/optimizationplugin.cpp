#include "optimizationplugin.h"
#include "optimizationoutput.h"

OptimizationPlugin::OptimizationPlugin(QObject *parent) : QObject(parent)
{
    
}

bool OptimizationPlugin::isCompilerSupported(const QString &compilerId) const
{
    return m_supportedCompilers.contains(compilerId);
}

bool OptimizationPlugin::isSessionEnabled() const
{
    return false;
}

bool OptimizationPlugin::hasResult(const QJsonObject &docObj) const
{
    QJsonValue optOutputVal = docObj["optOutput"];
    return optOutputVal.isArray();
}

QWidget *OptimizationPlugin::createOutputWidget(QWidget *parent)
{
    return new OptimizationOutput(parent);
}
