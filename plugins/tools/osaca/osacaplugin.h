#ifndef OSACAPLUGIN_H
#define OSACAPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "tools.h"

class OSACAPlugin
    : public QObject
    , public ToolInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.ToolInterface" FILE "osaca.json")
    Q_INTERFACES(ToolInterface)
public:
    explicit OSACAPlugin(QObject *parent = nullptr);
    ~OSACAPlugin() override {}

    [[nodiscard]] bool     isCurrentCompilerSupported() const override;
    [[nodiscard]] bool     isCurrentSessionEnabled() const override;
    [[nodiscard]] bool     hasResult() const override;
    [[nodiscard]] QWidget *createOutputWidget(QWidget *parent = nullptr) override;

signals:
};

#endif // OSACAPLUGIN_H
