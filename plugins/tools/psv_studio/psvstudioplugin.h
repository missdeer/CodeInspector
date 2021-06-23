#ifndef PSVSTUDIOPLUGIN_H
#define PSVSTUDIOPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "tools.h"

class PSVStudioPlugin
    : public QObject
    , public ToolInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.ToolInterface" FILE "psv_studio.json")
    Q_INTERFACES(ToolInterface)
public:
    explicit PSVStudioPlugin(QObject *parent = nullptr);
    ~PSVStudioPlugin() override = default;

    [[nodiscard]] bool     isCompilerSupported(const QString &compilerId) const override;
    [[nodiscard]] bool     isSessionEnabled() const override;
    [[nodiscard]] bool     hasResult(const QJsonObject &docObj) const override;
    [[nodiscard]] QWidget *createOutputWidget(QWidget *parent = nullptr) override;

signals:

private:
    QStringList m_supportedCompilers;
};

#endif // PSVSTUDIOPLUGIN_H
