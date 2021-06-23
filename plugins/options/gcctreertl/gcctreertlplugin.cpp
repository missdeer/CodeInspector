#include "gcctreertlplugin.h"

GCCTreeRTLPlugin::GCCTreeRTLPlugin(QObject *parent) : QObject(parent)
{
    
}

bool GCCTreeRTLPlugin::isCurrentCompilerSupported() const
{
    return false;
}

bool GCCTreeRTLPlugin::isCurrentSessionEnabled() const
{
    return false;
}

bool GCCTreeRTLPlugin::hasResult() const
{
    return false;
}

QWidget *GCCTreeRTLPlugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
