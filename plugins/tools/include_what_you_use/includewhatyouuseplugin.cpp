#include "includewhatyouuseplugin.h"

IncludeWhatYouUsePlugin::IncludeWhatYouUsePlugin(QObject *parent) : QObject(parent)
{
    
}

bool IncludeWhatYouUsePlugin::isCurrentCompilerSupported() const
{
    return false;
}

bool IncludeWhatYouUsePlugin::isCurrentSessionEnabled() const
{
    return false;
}

bool IncludeWhatYouUsePlugin::hasResult() const
{
    return false;
}

QWidget *IncludeWhatYouUsePlugin::outputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
