#include "psvstudioplugin.h"

PSVStudioPlugin::PSVStudioPlugin(QObject *parent) : QObject(parent)
{
    
}

bool PSVStudioPlugin::isCompilerSupported(int compilerId) const
{
    return false;
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
