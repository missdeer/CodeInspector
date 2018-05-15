#ifndef LIBRARYVERSION_H
#define LIBRARYVERSION_H

#include <QObject>
#include <QSharedPointer>

class LibraryVersion : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString version READ getVersion WRITE setVersion NOTIFY versionChanged)
    Q_PROPERTY(QStringList path READ getPath WRITE setPath NOTIFY pathChanged)
public:
    explicit LibraryVersion(QObject *parent = nullptr);

    const QString& getVersion() const;
    void setVersion(const QString &value);

    const QStringList& getPath() const;
    void setPath(const QStringList &value);
    void appendPath(const QString& p);
signals:
    void versionChanged();
    void pathChanged();
public slots:

private:
    QString version;
    QStringList path;
};

typedef QSharedPointer<LibraryVersion> LibraryVersionPtr;
#endif // LIBRARYVERSION_H
