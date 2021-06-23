#ifndef INCLUDEWHATYOUUSEPLUGIN_H
#define INCLUDEWHATYOUUSEPLUGIN_H

#include <QObject>
#include <QtPlugin>

#include "tools.h"

class IncludeWhatYouUsePlugin
    : public QObject
    , public ToolInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ismisv.codeinspector.ToolInterface" FILE "include_what_you_use.json")
    Q_INTERFACES(ToolInterface)
public:
    explicit IncludeWhatYouUsePlugin(QObject *parent = nullptr);
    ~IncludeWhatYouUsePlugin() override {}

    [[nodiscard]] bool     isCompilerSupported(int compilerId) const override;
    [[nodiscard]] bool     isSessionEnabled() const override;
    [[nodiscard]] bool     hasResult() const override;
    [[nodiscard]] QWidget *createOutputWidget(QWidget *parent = nullptr) override;

signals:

};

#endif // INCLUDEWHATYOUUSEPLUGIN_H
