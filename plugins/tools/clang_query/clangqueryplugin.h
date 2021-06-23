#ifndef CLANGQUERYPLUGIN_H
#define CLANGQUERYPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "tools.h"

class ClangQueryPlugin
    : public QObject
    , public ToolInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.ToolInterface" FILE "clang_query.json")
    Q_INTERFACES(ToolInterface)
public:
    explicit ClangQueryPlugin(QObject *parent = nullptr);
    ~ClangQueryPlugin() override {}

    [[nodiscard]] bool     isCompilerSupported() const override;
    [[nodiscard]] bool     isSessionEnabled() const override;
    [[nodiscard]] bool     hasResult() const override;
    [[nodiscard]] QWidget *createOutputWidget(QWidget *parent = nullptr) override;

signals:

};

#endif // CLANGQUERYPLUGIN_H
