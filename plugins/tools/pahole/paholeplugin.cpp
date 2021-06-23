#include "paholeplugin.h"

PaholePlugin::PaholePlugin(QObject *parent) : QObject(parent)
{
    
}

bool PaholePlugin::isCurrentCompilerSupported() const
{
    return false;
}

bool PaholePlugin::isCurrentSessionEnabled() const
{
    return false;
}

bool PaholePlugin::hasResult() const
{
    return false;
}

QWidget *PaholePlugin::outputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
