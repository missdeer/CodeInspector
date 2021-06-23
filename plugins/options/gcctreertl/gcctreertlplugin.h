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
    ~GCCTreeRTLPlugin() override = default;

    [[nodiscard]] bool     isCompilerSupported(const QString &compilerId) const override;
    [[nodiscard]] bool     isSessionEnabled() const override;
    [[nodiscard]] bool     hasResult(const QJsonObject& jsonObj) const override;
    [[nodiscard]] QWidget *createOutputWidget(QWidget *parent = nullptr) override;

signals:

private:
    QStringList m_supportedCompilers;
};

#endif // GCCTREERTLPLUGIN_H
