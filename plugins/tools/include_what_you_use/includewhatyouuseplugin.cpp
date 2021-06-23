#include "includewhatyouuseplugin.h"

IncludeWhatYouUsePlugin::IncludeWhatYouUsePlugin(QObject *parent) : QObject(parent)
{
    
}

bool IncludeWhatYouUsePlugin::isCompilerSupported(int compilerId) const
{
    return false;
}

bool IncludeWhatYouUsePlugin::isSessionEnabled() const
{
    return false;
}

bool IncludeWhatYouUsePlugin::hasResult(const QJsonObject& jsonObj) const
{
    return false;
}

QWidget *IncludeWhatYouUsePlugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
