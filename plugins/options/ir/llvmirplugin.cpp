#include "llvmirplugin.h"
#include "llvmirview.h"

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

bool LLVMIRPlugin::hasResult(const QJsonObject &docObj) const
{
    QJsonValue hasIrOutputVal = docObj["hasIrOutput"];
    if (hasIrOutputVal.isBool() && hasIrOutputVal.toBool())
    {
        QJsonValue irOutputVal = docObj["irOutput"];
        return irOutputVal.isArray();
    }
    return false;
}

QWidget *LLVMIRPlugin::createOutputWidget(QWidget *parent)
{
    return new LLVMIRView(parent);
}
