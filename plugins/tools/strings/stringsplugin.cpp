#include "stringsplugin.h"

StringsPlugin::StringsPlugin(QObject *parent) : QObject(parent)
{
    
}

bool StringsPlugin::isCompilerSupported(const QString &compilerId) const
{
    return m_supportedCompilers.contains(compilerId);
}

bool StringsPlugin::isSessionEnabled() const
{
    return false;
}

bool StringsPlugin::hasResult(const QJsonObject &docObj) const
{
    return false;
}

QWidget *StringsPlugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
