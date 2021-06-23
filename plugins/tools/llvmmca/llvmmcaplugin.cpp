#include "llvmmcaplugin.h"

LLVMMCAPlugin::LLVMMCAPlugin(QObject *parent) : QObject(parent)
{
    
}

bool LLVMMCAPlugin::isCompilerSupported(int compilerId) const
{
    return false;
}

bool LLVMMCAPlugin::isSessionEnabled() const
{
    return false;
}

bool LLVMMCAPlugin::hasResult() const
{
    return false;
}

QWidget *LLVMMCAPlugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
