#ifndef OSACAPLUGIN_H
#define OSACAPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "tools.h"

class OSACAPlugin
    : public QObject
    , public ToolInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.ToolInterface" FILE "osaca.json")
    Q_INTERFACES(ToolInterface)
public:
    explicit OSACAPlugin(QObject *parent = nullptr);
    ~OSACAPlugin() override {}

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

#endif // OSACAPLUGIN_H
