#include "clangqueryplugin.h"

ClangQueryPlugin::ClangQueryPlugin(QObject *parent) : QObject(parent)
{
    
}

bool ClangQueryPlugin::isCurrentCompilerSupported() const
{
    return false;
}

bool ClangQueryPlugin::isCurrentSessionEnabled() const
{
    return false;
}

bool ClangQueryPlugin::hasResult() const
{
    return false;
}

QWidget *ClangQueryPlugin::outputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
