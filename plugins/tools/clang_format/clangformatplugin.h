#ifndef CLANGFORMATPLUGIN_H
#define CLANGFORMATPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "tools.h"

class ClangFormatPlugin
    : public QObject
    , public ToolInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.ToolInterface" FILE "clang_format.json")
    Q_INTERFACES(ToolInterface)
public:
    explicit ClangFormatPlugin(QObject *parent = nullptr);
    ~ClangFormatPlugin() override {}

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

#endif // CLANGFORMATPLUGIN_H
