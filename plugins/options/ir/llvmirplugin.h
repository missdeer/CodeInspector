#ifndef LLVMIRPLUGIN_H
#define LLVMIRPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "options.h"

class LLVMIRPlugin
    : public QObject
    , public OptionInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.OptionInterface" FILE "ir.json")
    Q_INTERFACES(OptionInterface)
public:
    explicit LLVMIRPlugin(QObject *parent = nullptr);
    ~LLVMIRPlugin() override {}

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

#endif // LLVMIRPLUGIN_H
