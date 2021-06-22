#ifndef STRINGSPLUGIN_H
#define STRINGSPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "tools.h"

class StringsPlugin
    : public QObject
    , public ToolInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.ToolInterface" FILE "strings.json")
    Q_INTERFACES(ToolInterface)
public:
    explicit StringsPlugin(QObject *parent = nullptr);
    ~StringsPlugin() override {}

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

#endif // STRINGSPLUGIN_H
