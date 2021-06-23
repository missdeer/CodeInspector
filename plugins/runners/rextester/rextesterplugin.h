#ifndef REXTESTERPLUGIN_H
#define REXTESTERPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "runners.h"

class RextesterPlugin
    : public QObject
    , public RunnerInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.RunnerInterface" FILE "rextester.json")
    Q_INTERFACES(RunnerInterface)
public:
    explicit RextesterPlugin(QObject *parent = nullptr);
    ~RextesterPlugin() override = default;

    [[nodiscard]] bool isCompilerSupported(const QString &compilerId) const override;

signals:

private:
    QStringList m_supportedCompilers;
};

#endif // REXTESTERPLUGIN_H
