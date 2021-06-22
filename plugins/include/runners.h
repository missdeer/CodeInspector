#ifndef RUNNERS_INTERFACE_H
#define RUNNERS_INTERFACE_H

#include <QtPlugin>

class RunnerInterface
{
public:
    virtual ~RunnerInterface()       = default;
    virtual bool isSupported() const = 0;
};

QT_BEGIN_NAMESPACE
#define RunnerInterface_iid "com.ismisv.codeinspector.RunnerInterface/1.0"

Q_DECLARE_INTERFACE(RunnerInterface, RunnerInterface_iid)
QT_END_NAMESPACE

#endif
