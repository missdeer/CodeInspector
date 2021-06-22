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

    [[nodiscard]] bool isSupported() const override
    {
        return false;
    }
    [[nodiscard]] QWidget *outputWidget(QWidget *parent = nullptr) override
    {
        Q_UNUSED(parent);
        return nullptr;
    }
signals:

};

#endif // ASTPLUGIN_H
