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
    ~AstPlugin() override {}

    [[nodiscard]] bool     isCurrentCompilerSupported() const override;
    [[nodiscard]] bool     isCurrentSessionEnabled() const override;
    [[nodiscard]] bool     hasResult() const override;
    [[nodiscard]] QWidget *outputWidget(QWidget *parent = nullptr) override;
signals:

};

#endif // ASTPLUGIN_H
