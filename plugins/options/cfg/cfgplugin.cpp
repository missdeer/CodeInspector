#include "cfgplugin.h"

CfgPlugin::CfgPlugin(QObject *parent) : QObject(parent)
{
    
}

bool CfgPlugin::isCompilerSupported(const QString &compilerId) const
{
    return m_supportedCompilers.contains(compilerId);
}

bool CfgPlugin::isSessionEnabled() const
{
    return false;
}

bool CfgPlugin::hasResult(const QJsonObject& jsonObj) const
{
    return false;
}

QWidget *CfgPlugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
