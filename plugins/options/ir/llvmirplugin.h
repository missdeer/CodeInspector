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

    [[nodiscard]] bool     isCompilerSupported(int compilerId) const override;
    [[nodiscard]] bool     isSessionEnabled() const override;
    [[nodiscard]] bool     hasResult(const QJsonObject& jsonObj) const override;
    [[nodiscard]] QWidget *createOutputWidget(QWidget *parent = nullptr) override;

signals:

};

#endif // LLVMIRPLUGIN_H
