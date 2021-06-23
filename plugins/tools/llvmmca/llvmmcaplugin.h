#ifndef LLVMMCAPLUGIN_H
#define LLVMMCAPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "tools.h"

class LLVMMCAPlugin
    : public QObject
    , public ToolInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.ToolInterface" FILE "llvmmca.json")
    Q_INTERFACES(ToolInterface)
public:
    explicit LLVMMCAPlugin(QObject *parent = nullptr);
    ~LLVMMCAPlugin() override = default;

    [[nodiscard]] bool     isCompilerSupported(const QString &compilerId) const override;
    [[nodiscard]] bool     isSessionEnabled() const override;
    [[nodiscard]] bool     hasResult(const QJsonObject &docObj) const override;
    [[nodiscard]] QWidget *createOutputWidget(QWidget *parent = nullptr) override;

signals:

private:
    QStringList m_supportedCompilers;
};

#endif // LLVMMCAPLUGIN_H
