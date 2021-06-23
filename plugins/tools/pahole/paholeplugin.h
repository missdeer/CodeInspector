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
    ~PaholePlugin() override = default;

    [[nodiscard]] bool     isCompilerSupported(const QString &compilerId) const override;
    [[nodiscard]] bool     isSessionEnabled() const override;
    [[nodiscard]] bool     hasResult(const QJsonObject& jsonObj) const override;
    [[nodiscard]] QWidget *createOutputWidget(QWidget *parent = nullptr) override;

signals:

private:
    QStringList m_supportedCompilers;
};

#endif // PAHOLEPLUGIN_H
