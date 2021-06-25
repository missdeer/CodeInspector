#include "clangformatplugin.h"

ClangFormatPlugin::ClangFormatPlugin(QObject *parent) : QObject(parent)
{
    
}

bool ClangFormatPlugin::isCompilerSupported(const QString &compilerId) const
{
    return m_supportedCompilers.contains(compilerId);
}

bool ClangFormatPlugin::isSessionEnabled() const
{
    return false;
}

bool ClangFormatPlugin::hasResult(const QJsonObject &docObj, QString& stdOut, QString& stdErr) const
{
    return false;
}

QWidget *ClangFormatPlugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
