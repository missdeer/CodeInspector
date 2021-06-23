#ifndef ASTPLUGIN_H
#define ASTPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "options.h"

class AstPlugin
    : public QObject
    , public OptionInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.OptionInterface" FILE "ast.json")
    Q_INTERFACES(OptionInterface)
public:
    explicit AstPlugin(QObject *parent = nullptr);
    ~AstPlugin() override = default;

    [[nodiscard]] bool     isCompilerSupported(int compilerId) const override;
    [[nodiscard]] bool     isSessionEnabled() const override;
    [[nodiscard]] bool     hasResult(const QJsonObject& jsonObj) const override;
    [[nodiscard]] QWidget *createOutputWidget(QWidget *parent = nullptr) override;
signals:

};

#endif // ASTPLUGIN_H
