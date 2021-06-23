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

    [[nodiscard]] bool     isCurrentCompilerSupported() const override;
    [[nodiscard]] bool     isCurrentSessionEnabled() const override;
    [[nodiscard]] bool     hasResult() const override;
    [[nodiscard]] QWidget *createOutputWidget(QWidget *parent = nullptr) override;

signals:

};

#endif // CLANGFORMATPLUGIN_H
