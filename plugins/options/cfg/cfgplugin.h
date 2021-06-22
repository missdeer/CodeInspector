#ifndef CFGPLUGIN_H
#define CFGPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "options.h"

class CfgPlugin
    : public QObject
    , public OptionInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.OptionInterface" FILE "cfg.json")
    Q_INTERFACES(OptionInterface)
public:
    explicit CfgPlugin(QObject *parent = nullptr);
    ~CfgPlugin() override {}

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

#endif // CFGPLUGIN_H
