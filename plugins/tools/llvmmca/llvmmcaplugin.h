#ifndef LLVMMCAPLUGIN_H
#define LLVMMCAPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "tools.h"

class LLVMMCAPlugin
    : public QObject
    , public ToolInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.ToolInterface" FILE "llvmmca.json")
    Q_INTERFACES(ToolInterface)
public:
    explicit LLVMMCAPlugin(QObject *parent = nullptr);
    ~LLVMMCAPlugin() override {}

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

#endif // LLVMMCAPLUGIN_H
