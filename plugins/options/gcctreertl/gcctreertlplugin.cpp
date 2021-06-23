#include "gcctreertlplugin.h"

GCCTreeRTLPlugin::GCCTreeRTLPlugin(QObject *parent) : QObject(parent)
{
    
}

bool GCCTreeRTLPlugin::isCompilerSupported(const QString &compilerId) const
{
    return m_supportedCompilers.contains(compilerId);
}

bool GCCTreeRTLPlugin::isSessionEnabled() const
{
    return false;
}

bool GCCTreeRTLPlugin::hasResult(const QJsonObject &docObj) const
{
    return false;
}

QWidget *GCCTreeRTLPlugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
