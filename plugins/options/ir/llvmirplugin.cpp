#include "llvmirplugin.h"

LLVMIRPlugin::LLVMIRPlugin(QObject *parent) : QObject(parent)
{
    
}

bool LLVMIRPlugin::isCompilerSupported(const QString &compilerId) const
{
    return m_supportedCompilers.contains(compilerId);
}

bool LLVMIRPlugin::isSessionEnabled() const
{
    return false;
}

bool LLVMIRPlugin::hasResult(const QJsonObject& jsonObj) const
{
    return false;
}

QWidget *LLVMIRPlugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
