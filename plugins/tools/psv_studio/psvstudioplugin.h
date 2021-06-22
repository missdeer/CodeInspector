#ifndef PSVSTUDIOPLUGIN_H
#define PSVSTUDIOPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "tools.h"

class PSVStudioPlugin
    : public QObject
    , public ToolInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.ToolInterface" FILE "psv_studio.json")
    Q_INTERFACES(ToolInterface)
public:
    explicit PSVStudioPlugin(QObject *parent = nullptr);
    ~PSVStudioPlugin() override {}

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

#endif // PSVSTUDIOPLUGIN_H
