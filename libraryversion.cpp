#include "libraryversion.h"

LibraryVersion::LibraryVersion(QObject *parent) : QObject(parent)
{

}

const QString &LibraryVersion::getVersion() const
{
    return version;
}

void LibraryVersion::setVersion(const QString &value)
{
    version = value;
    emit versionChanged();
}

const QStringList &LibraryVersion::getPath() const
{
    return path;
}

void LibraryVersion::setPath(const QStringList &value)
{
    path = value;
    emit pathChanged();
}

void LibraryVersion::appendPath(const QString &p)
{
    path.append(p);
    emit pathChanged();
}
