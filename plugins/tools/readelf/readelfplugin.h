#ifndef READELFPLUGIN_H
#define READELFPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "tools.h"

class ReadELFPlugin
    : public QObject
    , public ToolInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.ToolInterface" FILE "readelf.json")
    Q_INTERFACES(ToolInterface)
public:
    explicit ReadELFPlugin(QObject *parent = nullptr);
    ~ReadELFPlugin() override = default;

    [[nodiscard]] bool     isCompilerSupported(const QString &compilerId) const override;
    [[nodiscard]] bool     isSessionEnabled() const override;
    [[nodiscard]] bool     hasResult(const QJsonObject &docObj) const override;
    [[nodiscard]] QWidget *createOutputWidget(QWidget *parent = nullptr) override;

signals:

private:
    QStringList m_supportedCompilers;
};

#endif // READELFPLUGIN_H
