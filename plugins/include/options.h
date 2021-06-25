#ifndef OPTIONS_INTERFACE_H
#define OPTIONS_INTERFACE_H

#include <QtPlugin>

QT_FORWARD_DECLARE_CLASS(QWidget);

class OptionInterface
{
public:
    virtual ~OptionInterface()                                                                    = default;
    virtual bool     isCompilerSupported(const QString &compilerId) const                         = 0;
    virtual bool     isSessionEnabled() const                                                     = 0;
    virtual bool     hasResult(const QJsonObject &docObj, QString &stdOut, QString &stdErr) const = 0;
    virtual QWidget *createOutputWidget(QWidget *parent = nullptr)                                = 0;
};

QT_BEGIN_NAMESPACE
#define OptionInterface_iid "com.ismisv.codeinspector.OptionInterface/1.0"

Q_DECLARE_INTERFACE(OptionInterface, OptionInterface_iid)
QT_END_NAMESPACE

#endif
