#include "clangtidyplugin.h"

ClangTidyPlugin::ClangTidyPlugin(QObject *parent) : QObject(parent)
{
    
}

bool ClangTidyPlugin::isCurrentCompilerSupported() const
{
    return false;
}

bool ClangTidyPlugin::isCurrentSessionEnabled() const
{
    return false;
}

bool ClangTidyPlugin::hasResult() const
{
    return false;
}

QWidget *ClangTidyPlugin::outputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
