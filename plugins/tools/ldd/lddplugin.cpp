#include "lddplugin.h"

LDDPlugin::LDDPlugin(QObject *parent) : QObject(parent)
{
    
}

bool LDDPlugin::isCompilerSupported(const QString &compilerId) const
{
    return m_supportedCompilers.contains(compilerId);
}

bool LDDPlugin::isSessionEnabled() const
{
    return false;
}

bool LDDPlugin::hasResult(const QJsonObject& jsonObj) const
{
    return false;
}

QWidget *LDDPlugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
