#include "psvstudioplugin.h"

PSVStudioPlugin::PSVStudioPlugin(QObject *parent) : QObject(parent)
{
    
}

bool PSVStudioPlugin::isCompilerSupported(const QString &compilerId) const
{
    return m_supportedCompilers.contains(compilerId);
}

bool PSVStudioPlugin::isSessionEnabled() const
{
    return false;
}

bool PSVStudioPlugin::hasResult(const QJsonObject& jsonObj) const
{
    return false;
}

QWidget *PSVStudioPlugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
