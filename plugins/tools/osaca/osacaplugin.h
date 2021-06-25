#ifndef OSACAPLUGIN_H
#define OSACAPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "tools.h"

class OSACAPlugin
    : public QObject
    , public ToolInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.ToolInterface" FILE "osaca.json")
    Q_INTERFACES(ToolInterface)
public:
    explicit OSACAPlugin(QObject *parent = nullptr);
    ~OSACAPlugin() override = default;

    [[nodiscard]] bool     isCompilerSupported(const QString &compilerId) const override;
    [[nodiscard]] bool     isSessionEnabled() const override;
    [[nodiscard]] bool     hasResult(const QJsonObject &docObj, QString& stdOut, QString& stdErr) const override;
    [[nodiscard]] QWidget *createOutputWidget(QWidget *parent = nullptr) override;

signals:

private:
    QStringList m_supportedCompilers;
};

#endif // OSACAPLUGIN_H
