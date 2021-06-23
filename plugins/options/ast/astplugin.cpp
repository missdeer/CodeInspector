#include "astplugin.h"

AstPlugin::AstPlugin(QObject *parent) : QObject(parent)
{
    
}

bool AstPlugin::isCompilerSupported(const QString &compilerId) const
{
    return m_supportedCompilers.contains(compilerId);
}

bool AstPlugin::isSessionEnabled() const
{
    return false;
}

bool AstPlugin::hasResult(const QJsonObject& jsonObj) const
{
    return false;
}

QWidget *AstPlugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
