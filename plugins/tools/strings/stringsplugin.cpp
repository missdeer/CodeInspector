#include "stringsplugin.h"

StringsPlugin::StringsPlugin(QObject *parent) : QObject(parent)
{
    
}

bool StringsPlugin::isCurrentCompilerSupported() const
{
    return false;
}

bool StringsPlugin::isCurrentSessionEnabled() const
{
    return false;
}

bool StringsPlugin::hasResult() const
{
    return false;
}

QWidget *StringsPlugin::outputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
