#ifndef LIBRARYVERSION_H
#define LIBRARYVERSION_H

#include <QObject>
#include <QSharedPointer>

class LibraryVersion : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString version READ getVersion WRITE setVersion NOTIFY versionChanged)
    Q_PROPERTY(QStringList path READ getPath WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(bool selected READ getSelected WRITE setSelected NOTIFY selectedChanged)
public:
    explicit LibraryVersion(QObject *parent = nullptr);

    const QString& getVersion() const;
    void setVersion(const QString &value);

    const QStringList& getPath() const;
    void setPath(const QStringList &value);
    void appendPath(const QString& p);

    bool getSelected() const;
    void setSelected(bool value);

signals:
    void versionChanged();
    void pathChanged();
    void selectedChanged();
public slots:

private:
    bool selected;
    QString version;
    QStringList path;
};

using LibraryVersionPtr = QSharedPointer<LibraryVersion>;
#endif // LIBRARYVERSION_H
