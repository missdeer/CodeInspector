#include "stringsplugin.h"

StringsPlugin::StringsPlugin(QObject *parent) : QObject(parent)
{
    
}

bool StringsPlugin::isCompilerSupported(int compilerId) const
{
    return false;
}

bool StringsPlugin::isSessionEnabled() const
{
    return false;
}

bool StringsPlugin::hasResult(const QJsonObject& jsonObj) const
{
    return false;
}

QWidget *StringsPlugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
