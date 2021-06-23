#ifndef X86TO6502PLUGIN_H
#define X86TO6502PLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "tools.h"

class X86To6502Plugin
    : public QObject
    , public ToolInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.ToolInterface" FILE "x86to6502.json")
    Q_INTERFACES(ToolInterface)
public:
    explicit X86To6502Plugin(QObject *parent = nullptr);
    ~X86To6502Plugin() override = default;

    [[nodiscard]] bool     isCompilerSupported(const QString &compilerId) const override;
    [[nodiscard]] bool     isSessionEnabled() const override;
    [[nodiscard]] bool     hasResult(const QJsonObject& jsonObj) const override;
    [[nodiscard]] QWidget *createOutputWidget(QWidget *parent = nullptr) override;

signals:

private:
    QStringList m_supportedCompilers;
};

#endif // X86TO6502PLUGIN_H
