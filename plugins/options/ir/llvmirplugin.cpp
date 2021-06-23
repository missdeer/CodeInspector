#include "llvmirplugin.h"

LLVMIRPlugin::LLVMIRPlugin(QObject *parent) : QObject(parent)
{
    
}

bool LLVMIRPlugin::isCurrentCompilerSupported() const
{
    return false;
}

bool LLVMIRPlugin::isCurrentSessionEnabled() const
{
    return false;
}

bool LLVMIRPlugin::hasResult() const
{
    return false;
}

QWidget *LLVMIRPlugin::outputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
