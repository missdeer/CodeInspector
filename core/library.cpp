#include "stdafx.h"

#include "library.h"

Library::Library(QObject *parent) : QObject(parent) {}

const QString &Library::getId() const
{
    return id;
}

void Library::setId(const QString &value)
{
    id = value;
    emit idChanged();
}

const QString &Library::getName() const
{
    return name;
}

void Library::setName(const QString &value)
{
    name = value;
    emit nameChanged();
}

const QString &Library::getDescription() const
{
    return description;
}

void Library::setDescription(const QString &value)
{
    description = value;
    emit descriptionChanged();
}

const QString &Library::getUrl() const
{
    return url;
}

void Library::setUrl(const QString &value)
{
    url = value;
    emit urlChanged();
}

const QList<LibraryVersionPtr> &Library::getVersions()
{
    return versions;
}

void Library::setVersions(const QList<LibraryVersionPtr> &value)
{
    versions = value;
    emit versionsChanged();
}

void Library::appendVersion(LibraryVersionPtr ver)
{
    versions.append(ver);
    emit versionsChanged();
}

#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
QQmlListProperty<LibraryVersion> Library::getQmlListPropertyVersions()
{
    return QQmlListProperty<LibraryVersion>(
        this, this, &Library::appendLibVersion, &Library::libVersionCount, &Library::libVersion, &Library::clearLibVersions);
}

void Library::appendLibVersion(QQmlListProperty<LibraryVersion> *list, LibraryVersion *p)
{
    reinterpret_cast<Library *>(list->data)->appendLibVersion(p);
}

int Library::libVersionCount(QQmlListProperty<LibraryVersion> *list)
{
    return reinterpret_cast<Library *>(list->data)->libVersionCount();
}

LibraryVersion *Library::libVersion(QQmlListProperty<LibraryVersion> *list, int index)
{
    return reinterpret_cast<Library *>(list->data)->libVersion(index);
}

void Library::clearLibVersions(QQmlListProperty<LibraryVersion> *list)
{
    reinterpret_cast<Library *>(list->data)->clearLibVersions();
}

void Library::appendLibVersion(LibraryVersion *) {}

int Library::libVersionCount() const
{
    return versions.length();
}

LibraryVersion *Library::libVersion(int index) const
{
    if (index >= versions.length())
        return nullptr;
    return versions.at(index).data();
}

void Library::clearLibVersions() {}
#endif
