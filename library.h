#ifndef LIBRARY_H
#define LIBRARY_H

#include <QObject>
#include <QQmlListProperty>
#include "libraryversion.h"

class Library : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ getId WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString url READ getUrl WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QQmlListProperty<LibraryVersion> versions READ getQmlListPropertyVersions NOTIFY versionsChanged)
public:
    explicit Library(QObject *parent = nullptr);

    const QString& getId() const;
    void setId(const QString &value);

    const QString& getName() const;
    void setName(const QString &value);

    const QString& getDescription() const;
    void setDescription(const QString &value);

    const QString& getUrl() const;
    void setUrl(const QString &value);

    QQmlListProperty<LibraryVersion> getQmlListPropertyVersions();
    const QList<LibraryVersionPtr> &getVersions();
    void setVersions(const QList<LibraryVersionPtr> &value);
    void appendVersion(LibraryVersionPtr ver);
signals:
    void idChanged();
    void nameChanged();
    void descriptionChanged();
    void urlChanged();
    void versionsChanged();
public slots:

private:
    QString id;
    QString name;
    QString description;
    QString url;
    QList<LibraryVersionPtr> versions;

    static void appendLibVersion(QQmlListProperty<LibraryVersion>* list, LibraryVersion* p);
    static int libVersionCount(QQmlListProperty<LibraryVersion>* list);
    static LibraryVersion *libVersion(QQmlListProperty<LibraryVersion>* list, int index);
    static void clearLibVersions(QQmlListProperty<LibraryVersion>* list);

    void appendLibVersion(LibraryVersion* p);
    int libVersionCount() const;
    LibraryVersion *libVersion(int index) const;
    void clearLibVersions();
};

typedef QSharedPointer<Library> LibraryPtr;
typedef QList<LibraryPtr> LibraryList;
typedef QSharedPointer<LibraryList> LibraryListPtr;
#endif // LIBRARY_H