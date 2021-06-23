#include "includewhatyouuseplugin.h"

IncludeWhatYouUsePlugin::IncludeWhatYouUsePlugin(QObject *parent) : QObject(parent)
{
    
}

bool IncludeWhatYouUsePlugin::isCompilerSupported(const QString &compilerId) const
{
    return m_supportedCompilers.contains(compilerId);
}

bool IncludeWhatYouUsePlugin::isSessionEnabled() const
{
    return false;
}

bool IncludeWhatYouUsePlugin::hasResult(const QJsonObject &docObj) const
{
    return false;
}

QWidget *IncludeWhatYouUsePlugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
