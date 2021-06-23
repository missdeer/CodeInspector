#ifndef GCCTREERTLPLUGIN_H
#define GCCTREERTLPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "options.h"

class GCCTreeRTLPlugin
    : public QObject
    , public OptionInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.OptionInterface" FILE "gcctreertl.json")
    Q_INTERFACES(OptionInterface)
public:
    explicit GCCTreeRTLPlugin(QObject *parent = nullptr);
    ~GCCTreeRTLPlugin() override {}

    [[nodiscard]] bool     isCurrentCompilerSupported() const override;
    [[nodiscard]] bool     isCurrentSessionEnabled() const override;
    [[nodiscard]] bool     hasResult() const override;
    [[nodiscard]] QWidget *outputWidget(QWidget *parent = nullptr) override;

signals:

};

#endif // GCCTREERTLPLUGIN_H
