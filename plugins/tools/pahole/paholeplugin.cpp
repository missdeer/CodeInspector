#include "paholeplugin.h"

PaholePlugin::PaholePlugin(QObject *parent) : QObject(parent)
{
    
}

bool PaholePlugin::isCompilerSupported(int compilerId) const
{
    return false;
}

bool PaholePlugin::isSessionEnabled() const
{
    return false;
}

bool PaholePlugin::hasResult() const
{
    return false;
}

QWidget *PaholePlugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
