#include "clangtidyplugin.h"

ClangTidyPlugin::ClangTidyPlugin(QObject *parent) : QObject(parent)
{
    
}

bool ClangTidyPlugin::isCompilerSupported(int compilerId) const
{
    return false;
}

bool ClangTidyPlugin::isSessionEnabled() const
{
    return false;
}

bool ClangTidyPlugin::hasResult(const QJsonObject& jsonObj) const
{
    return false;
}

QWidget *ClangTidyPlugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
