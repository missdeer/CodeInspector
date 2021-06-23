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
    ~OSACAPlugin() override = default;

    [[nodiscard]] bool     isCompilerSupported(int compilerId) const override;
    [[nodiscard]] bool     isSessionEnabled() const override;
    [[nodiscard]] bool     hasResult(const QJsonObject& jsonObj) const override;
    [[nodiscard]] QWidget *createOutputWidget(QWidget *parent = nullptr) override;

signals:
};

#endif // OSACAPLUGIN_H
