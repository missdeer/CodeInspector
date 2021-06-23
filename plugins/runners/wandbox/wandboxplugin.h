#ifndef WANDBOXPLUGIN_H
#define WANDBOXPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "runners.h"

class WandboxPlugin
    : public QObject
    , public RunnerInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.RunnerInterface" FILE "wandbox.json")
    Q_INTERFACES(RunnerInterface)
public:
    explicit WandboxPlugin(QObject *parent = nullptr);
    ~WandboxPlugin() override {}

    [[nodiscard]] bool isCurrentCompilerSupported() const override
    {
        return false;
    }

signals:

};

#endif // WANDBOXPLUGIN_H
