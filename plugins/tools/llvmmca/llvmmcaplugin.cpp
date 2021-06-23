#include "llvmmcaplugin.h"

LLVMMCAPlugin::LLVMMCAPlugin(QObject *parent) : QObject(parent)
{
    
}

bool LLVMMCAPlugin::isCompilerSupported(const QString &compilerId) const
{
    return m_supportedCompilers.contains(compilerId);
}

bool LLVMMCAPlugin::isSessionEnabled() const
{
    return false;
}

bool LLVMMCAPlugin::hasResult(const QJsonObject &docObj) const
{
    return false;
}

QWidget *LLVMMCAPlugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
