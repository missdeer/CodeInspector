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
    ~OptimizationPlugin() override = default;

    [[nodiscard]] bool     isCompilerSupported(const QString &compilerId) const override;
    [[nodiscard]] bool     isSessionEnabled() const override;
    [[nodiscard]] bool     hasResult(const QJsonObject &docObj, QString &stdOut, QString &stdErr) const override;
    [[nodiscard]] QWidget *createOutputWidget(QWidget *parent = nullptr) override;

signals:

private:
    QStringList m_supportedCompilers;
};

#endif // OPTIMIZATIONPLUGIN_H
