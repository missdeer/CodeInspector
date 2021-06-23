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
    ~CfgPlugin() override {}

    [[nodiscard]] bool     isCompilerSupported() const override;
    [[nodiscard]] bool     isSessionEnabled() const override;
    [[nodiscard]] bool     hasResult() const override;
    [[nodiscard]] QWidget *createOutputWidget(QWidget *parent = nullptr) override;

signals:

};

#endif // CFGPLUGIN_H
