#include "psvstudioplugin.h"

PSVStudioPlugin::PSVStudioPlugin(QObject *parent) : QObject(parent)
{
    
}

bool PSVStudioPlugin::isCurrentCompilerSupported() const
{
    return false;
}

bool PSVStudioPlugin::isCurrentSessionEnabled() const
{
    return false;
}

bool PSVStudioPlugin::hasResult() const
{
    return false;
}

QWidget *PSVStudioPlugin::outputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
