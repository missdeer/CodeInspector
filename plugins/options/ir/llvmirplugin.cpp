#include "llvmirplugin.h"

LLVMIRPlugin::LLVMIRPlugin(QObject *parent) : QObject(parent)
{
    
}

bool LLVMIRPlugin::isCompilerSupported(int compilerId) const
{
    return false;
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
