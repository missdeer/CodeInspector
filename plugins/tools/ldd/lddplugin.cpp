#include "lddplugin.h"
#include "lddoutput.h"

LDDPlugin::LDDPlugin(QObject *parent) : QObject(parent)
{
    
}

bool LDDPlugin::isCompilerSupported(const QString &compilerId) const
{
    return m_supportedCompilers.contains(compilerId);
}

bool LDDPlugin::isSessionEnabled() const
{
    return false;
}

bool LDDPlugin::hasResult(const QJsonObject &docObj) const
{
    return false;
}

QWidget *LDDPlugin::createOutputWidget(QWidget *parent)
{
    return new LddOutput(parent);
}
