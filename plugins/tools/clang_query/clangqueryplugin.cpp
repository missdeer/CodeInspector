#include "clangqueryplugin.h"
#include "clangqueryoutput.h"

ClangQueryPlugin::ClangQueryPlugin(QObject *parent) : QObject(parent)
{
    
}

bool ClangQueryPlugin::isCompilerSupported(const QString &compilerId) const
{
    return m_supportedCompilers.contains(compilerId);
}

bool ClangQueryPlugin::isSessionEnabled() const
{
    return false;
}

bool ClangQueryPlugin::hasResult(const QJsonObject &docObj) const
{
    return false;
}

QWidget *ClangQueryPlugin::createOutputWidget(QWidget *parent)
{
    return new ClangQueryOutput(parent);
}
