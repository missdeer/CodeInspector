#include "astplugin.h"
#include "astoutput.h"

AstPlugin::AstPlugin(QObject *parent) : QObject(parent)
{
    
}

bool AstPlugin::isCompilerSupported(const QString &compilerId) const
{
    return m_supportedCompilers.contains(compilerId);
}

bool AstPlugin::isSessionEnabled() const
{
    return false;
}

bool AstPlugin::hasResult(const QJsonObject &docObj) const
{
    return false;
}

QWidget *AstPlugin::createOutputWidget(QWidget *parent)
{
    return new ASTOutput(parent);
}
