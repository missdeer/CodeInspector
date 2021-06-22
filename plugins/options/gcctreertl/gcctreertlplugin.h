#ifndef GCCTREERTLPLUGIN_H
#define GCCTREERTLPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "options.h"

class GCCTreeRTLPlugin
    : public QObject
    , public OptionInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.OptionInterface" FILE "gcctreertl.json")
    Q_INTERFACES(OptionInterface)
public:
    explicit GCCTreeRTLPlugin(QObject *parent = nullptr);
    ~GCCTreeRTLPlugin() override {}

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

#endif // GCCTREERTLPLUGIN_H
