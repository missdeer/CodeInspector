#ifndef LLVMIRPLUGIN_H
#define LLVMIRPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "options.h"

class LLVMIRPlugin
    : public QObject
    , public OptionInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.OptionInterface" FILE "ir.json")
    Q_INTERFACES(OptionInterface)
public:
    explicit LLVMIRPlugin(QObject *parent = nullptr);
    ~LLVMIRPlugin() override = default;

    [[nodiscard]] bool     isCompilerSupported(const QString &compilerId) const override;
    [[nodiscard]] bool     isSessionEnabled() const override;
    [[nodiscard]] bool     hasResult(const QJsonObject &docObj, QString &stdOut, QString &stdErr) const override;
    [[nodiscard]] QWidget *createOutputWidget(QWidget *parent = nullptr) override;

signals:

private:
    QStringList m_supportedCompilers;
};

#endif // LLVMIRPLUGIN_H
