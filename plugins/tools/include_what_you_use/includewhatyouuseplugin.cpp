#include "includewhatyouuseplugin.h"

IncludeWhatYouUsePlugin::IncludeWhatYouUsePlugin(QObject *parent) : QObject(parent)
{
    
}

bool IncludeWhatYouUsePlugin::isCompilerSupported() const
{
    return false;
}

bool IncludeWhatYouUsePlugin::isSessionEnabled() const
{
    return false;
}

bool IncludeWhatYouUsePlugin::hasResult() const
{
    return false;
}

QWidget *IncludeWhatYouUsePlugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
