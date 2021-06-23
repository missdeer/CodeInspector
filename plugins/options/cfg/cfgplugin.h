#ifndef CFGPLUGIN_H
#define CFGPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "options.h"

class CfgPlugin
    : public QObject
    , public OptionInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.OptionInterface" FILE "cfg.json")
    Q_INTERFACES(OptionInterface)
public:
    explicit CfgPlugin(QObject *parent = nullptr);
    ~CfgPlugin() override = default;

    [[nodiscard]] bool     isCompilerSupported(int compilerId) const override;
    [[nodiscard]] bool     isSessionEnabled() const override;
    [[nodiscard]] bool     hasResult(const QJsonObject &jsonObj) const override;
    [[nodiscard]] QWidget *createOutputWidget(QWidget *parent = nullptr) override;

signals:
};

#endif // CFGPLUGIN_H
