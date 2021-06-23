#include "clangformatplugin.h"

ClangFormatPlugin::ClangFormatPlugin(QObject *parent) : QObject(parent)
{
    
}

bool ClangFormatPlugin::isCompilerSupported() const
{
    return false;
}

bool ClangFormatPlugin::isSessionEnabled() const
{
    return false;
}

bool ClangFormatPlugin::hasResult() const
{
    return false;
}

QWidget *ClangFormatPlugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
