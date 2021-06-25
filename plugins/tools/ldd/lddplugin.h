#ifndef LDDPLUGIN_H
#define LDDPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "tools.h"

class LDDPlugin
    : public QObject
    , public ToolInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.ToolInterface" FILE "ldd.json")
    Q_INTERFACES(ToolInterface)
public:
    explicit LDDPlugin(QObject *parent = nullptr);
    ~LDDPlugin() override = default;

    [[nodiscard]] bool     isCompilerSupported(const QString &compilerId) const override;
    [[nodiscard]] bool     isSessionEnabled() const override;
    [[nodiscard]] bool     hasResult(const QJsonObject &docObj, QString &stdOut, QString &stdErr) const override;
    [[nodiscard]] QWidget *createOutputWidget(QWidget *parent = nullptr) override;

signals:

private:
    QStringList m_supportedCompilers;
};

#endif // LDDPLUGIN_H
