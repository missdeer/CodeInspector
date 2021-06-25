#include "includewhatyouuseplugin.h"
#include "includewhatyouuseoutput.h"
#include "pluginbase.h"

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
    return PluginBase::hasToolResult(docObj, "iwyu");
}

QWidget *IncludeWhatYouUsePlugin::createOutputWidget(QWidget *parent)
{
    return new IncludeWhatYouUseOutput(parent);
}
