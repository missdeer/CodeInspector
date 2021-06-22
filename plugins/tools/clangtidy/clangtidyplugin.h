#ifndef CLANGTIDYPLUGIN_H
#define CLANGTIDYPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "tools.h"

class ClangTidyPlugin
    : public QObject
    , public ToolInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.ToolInterface" FILE "clangtidy.json")
    Q_INTERFACES(ToolInterface)
public:
    explicit ClangTidyPlugin(QObject *parent = nullptr);
    ~ClangTidyPlugin() override {}

    [[nodiscard]] bool isSupported() const override
    {
        return false;
    }
    [[nodiscard]] QWidget *outputWidget(QWidget *parent = nullptr) override
    {
        Q_UNUSED(parent);
        return nullptr;
    }

signals:

};

#endif // CLANGTIDYPLUGIN_H
