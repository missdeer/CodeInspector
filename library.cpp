#include "stdafx.h"
#include "library.h"

Library::Library(QObject *parent) : QObject(parent)
{

}

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

QQmlListProperty<LibraryVersion> Library::getVersions()
{
    m_versions.clear();
    for (auto ver : versions)
    {
        m_versions.append(ver.data());
    }
    return QQmlListProperty<LibraryVersion>(this, m_versions);
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
