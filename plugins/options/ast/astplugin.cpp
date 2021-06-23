#include "astplugin.h"

AstPlugin::AstPlugin(QObject *parent) : QObject(parent)
{
    
}

bool AstPlugin::isCurrentCompilerSupported() const
{
    return false;
}

bool AstPlugin::isCurrentSessionEnabled() const
{
    return false;
}

bool AstPlugin::hasResult() const
{
    return false;
}

QWidget *AstPlugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
