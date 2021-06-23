#ifndef STRINGSPLUGIN_H
#define STRINGSPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "tools.h"

class StringsPlugin
    : public QObject
    , public ToolInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.ToolInterface" FILE "strings.json")
    Q_INTERFACES(ToolInterface)
public:
    explicit StringsPlugin(QObject *parent = nullptr);
    ~StringsPlugin() override = default;

    [[nodiscard]] bool     isCompilerSupported(const QString &compilerId) const override;
    [[nodiscard]] bool     isSessionEnabled() const override;
    [[nodiscard]] bool     hasResult(const QJsonObject &docObj) const override;
    [[nodiscard]] QWidget *createOutputWidget(QWidget *parent = nullptr) override;

signals:

private:
    QStringList m_supportedCompilers;
};

#endif // STRINGSPLUGIN_H
