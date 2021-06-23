#ifndef OPTIMIZATIONPLUGIN_H
#define OPTIMIZATIONPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "options.h"

class OptimizationPlugin
    : public QObject
    , public OptionInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.OptionInterface" FILE "optimization.json")
    Q_INTERFACES(OptionInterface)
public:
    explicit OptimizationPlugin(QObject *parent = nullptr);
    ~OptimizationPlugin() override {}

    [[nodiscard]] bool     isCurrentCompilerSupported() const override;
    [[nodiscard]] bool     isCurrentSessionEnabled() const override;
    [[nodiscard]] bool     hasResult() const override;
    [[nodiscard]] QWidget *createOutputWidget(QWidget *parent = nullptr) override;

signals:

};

#endif // OPTIMIZATIONPLUGIN_H
