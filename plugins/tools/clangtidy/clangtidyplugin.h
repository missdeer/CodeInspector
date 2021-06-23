#ifndef CLANGTIDYPLUGIN_H
#define CLANGTIDYPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "tools.h"

class ClangTidyPlugin
    : public QObject
    , public ToolInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.ToolInterface" FILE "clangtidy.json")
    Q_INTERFACES(ToolInterface)
public:
    explicit ClangTidyPlugin(QObject *parent = nullptr);
    ~ClangTidyPlugin() override {}

    [[nodiscard]] bool     isCompilerSupported() const override;
    [[nodiscard]] bool     isSessionEnabled() const override;
    [[nodiscard]] bool     hasResult() const override;
    [[nodiscard]] QWidget *createOutputWidget(QWidget *parent = nullptr) override;

signals:

};

#endif // CLANGTIDYPLUGIN_H
