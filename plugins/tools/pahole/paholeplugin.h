#ifndef PAHOLEPLUGIN_H
#define PAHOLEPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "tools.h"

class PaholePlugin
    : public QObject
    , public ToolInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.ToolInterface" FILE "pahole.json")
    Q_INTERFACES(ToolInterface)
public:
    explicit PaholePlugin(QObject *parent = nullptr);
    ~PaholePlugin() override {}

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

#endif // PAHOLEPLUGIN_H
