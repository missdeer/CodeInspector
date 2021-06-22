#ifndef READELFPLUGIN_H
#define READELFPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "tools.h"

class ReadELFPlugin
    : public QObject
    , public ToolInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.ToolInterface" FILE "readelf.json")
    Q_INTERFACES(ToolInterface)
public:
    explicit ReadELFPlugin(QObject *parent = nullptr);
    ~ReadELFPlugin() override {}

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

#endif // READELFPLUGIN_H
