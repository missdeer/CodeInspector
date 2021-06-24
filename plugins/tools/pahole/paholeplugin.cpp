#include "paholeplugin.h"
#include "paholeoutput.h"

PaholePlugin::PaholePlugin(QObject *parent) : QObject(parent)
{
    
}

bool PaholePlugin::isCompilerSupported(const QString &compilerId) const
{
    return m_supportedCompilers.contains(compilerId);
}

bool PaholePlugin::isSessionEnabled() const
{
    return false;
}

bool PaholePlugin::hasResult(const QJsonObject &docObj) const
{
    return false;
}

QWidget *PaholePlugin::createOutputWidget(QWidget *parent)
{
    return new PaholeOutput(parent);
}
