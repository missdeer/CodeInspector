#include "astplugin.h"

AstPlugin::AstPlugin(QObject *parent) : QObject(parent)
{
    
}

bool AstPlugin::isCompilerSupported(int compilerId) const
{
    return false;
}

bool AstPlugin::isSessionEnabled() const
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
