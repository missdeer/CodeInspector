#include "clangformatplugin.h"

ClangFormatPlugin::ClangFormatPlugin(QObject *parent) : QObject(parent)
{
    
}

bool ClangFormatPlugin::isCurrentCompilerSupported() const
{
    return false;
}

bool ClangFormatPlugin::isCurrentSessionEnabled() const
{
    return false;
}

bool ClangFormatPlugin::hasResult() const
{
    return false;
}

QWidget *ClangFormatPlugin::outputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
