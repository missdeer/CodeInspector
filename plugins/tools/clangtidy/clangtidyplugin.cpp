#include "clangtidyplugin.h"
#include "clangtidyoutput.h"
#include "pluginbase.h"

ClangTidyPlugin::ClangTidyPlugin(QObject *parent) : QObject(parent)
{
    
}

bool ClangTidyPlugin::isCompilerSupported(const QString &compilerId) const
{
    return m_supportedCompilers.contains(compilerId);
}

bool ClangTidyPlugin::isSessionEnabled() const
{
    return false;
}

bool ClangTidyPlugin::hasResult(const QJsonObject &docObj, QString &stdOut, QString &stdErr) const
{
    return PluginBase::hasToolResult(docObj, "clangtidytrunk", stdOut, stdErr);
}

QWidget *ClangTidyPlugin::createOutputWidget(QWidget *parent)
{
    return new ClangTidyOutput(parent);
}
