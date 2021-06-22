#ifndef X86TO6502PLUGIN_H
#define X86TO6502PLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "tools.h"

class X86To6502Plugin
    : public QObject
    , public ToolInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.ToolInterface" FILE "x86to6502.json")
    Q_INTERFACES(ToolInterface)
public:
    explicit X86To6502Plugin(QObject *parent = nullptr);
    ~X86To6502Plugin() override {}

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

#endif // X86TO6502PLUGIN_H
