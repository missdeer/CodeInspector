#include "clangqueryplugin.h"

ClangQueryPlugin::ClangQueryPlugin(QObject *parent) : QObject(parent)
{
    
}

bool ClangQueryPlugin::isCompilerSupported() const
{
    return false;
}

bool ClangQueryPlugin::isSessionEnabled() const
{
    return false;
}

bool ClangQueryPlugin::hasResult() const
{
    return false;
}

QWidget *ClangQueryPlugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
