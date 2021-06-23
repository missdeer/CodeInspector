#include "clangqueryplugin.h"

ClangQueryPlugin::ClangQueryPlugin(QObject *parent) : QObject(parent)
{
    
}

bool ClangQueryPlugin::isCompilerSupported(int compilerId) const
{
    return false;
}

bool ClangQueryPlugin::isSessionEnabled() const
{
    return false;
}

bool ClangQueryPlugin::hasResult(const QJsonObject& jsonObj) const
{
    return false;
}

QWidget *ClangQueryPlugin::createOutputWidget(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}
