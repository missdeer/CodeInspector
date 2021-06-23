#ifndef PAHOLEPLUGIN_H
#define PAHOLEPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "tools.h"

class PaholePlugin
    : public QObject
    , public ToolInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.ToolInterface" FILE "pahole.json")
    Q_INTERFACES(ToolInterface)
public:
    explicit PaholePlugin(QObject *parent = nullptr);
    ~PaholePlugin() override {}

    [[nodiscard]] bool     isCompilerSupported(int compilerId) const override;
    [[nodiscard]] bool     isSessionEnabled() const override;
    [[nodiscard]] bool     hasResult() const override;
    [[nodiscard]] QWidget *createOutputWidget(QWidget *parent = nullptr) override;

signals:
};

#endif // PAHOLEPLUGIN_H
