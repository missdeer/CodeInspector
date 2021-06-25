#ifndef TOOLS_INTERFACE_H
#define TOOLS_INTERFACE_H

#include <QtPlugin>

QT_FORWARD_DECLARE_CLASS(QWidget);

class ToolInterface
{
public:
    virtual ~ToolInterface()                                                                      = default;
    virtual bool     isCompilerSupported(const QString &compilerId) const                         = 0;
    virtual bool     isSessionEnabled() const                                                     = 0;
    virtual bool     hasResult(const QJsonObject &docObj, QString &stdOut, QString &stdErr) const = 0;
    virtual QWidget *createOutputWidget(QWidget *parent)                                          = 0;
};

QT_BEGIN_NAMESPACE
#define ToolInterface_iid "com.ismisv.codeinspector.ToolInterface/1.0"

Q_DECLARE_INTERFACE(ToolInterface, ToolInterface_iid)
QT_END_NAMESPACE

#endif
