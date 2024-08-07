#ifndef QUICKWIDGETAPI_H
#define QUICKWIDGETAPI_H

#include <QObject>
#include <QQmlEngine>
#include "library.h"

class QuickWidgetAPI : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool binary READ binary WRITE setBinary NOTIFY binaryChanged)
    Q_PROPERTY(bool functionsFromOtherLibraries READ functionsFromOtherLibraries WRITE setFunctionsFromOtherLibraries NOTIFY functionsFromOtherLibrariesChanged)
    Q_PROPERTY(bool labels READ labels WRITE setLabels NOTIFY labelsChanged)
    Q_PROPERTY(bool trim READ trim WRITE setTrim NOTIFY trimChanged)
    Q_PROPERTY(bool directives READ directives WRITE setDirectives NOTIFY directivesChanged)
    Q_PROPERTY(bool intel READ intel WRITE setIntel NOTIFY intelChanged)
    Q_PROPERTY(bool commentOnly READ commentOnly WRITE setCommentOnly NOTIFY commentOnlyChanged)
    Q_PROPERTY(bool demangle READ demangle WRITE setDemangle NOTIFY demangleChanged)
    Q_PROPERTY(bool binaryEnabled READ binaryEnabled WRITE setBinaryEnabled NOTIFY binaryEnabledChanged)
    Q_PROPERTY(bool functionsFromOtherLibrariesEnabled READ functionsFromOtherLibrariesEnabled WRITE setFunctionsFromOtherLibrariesEnabled NOTIFY functionsFromOtherLibrariesEnabledChanged)
    Q_PROPERTY(bool labelsEnabled READ labelsEnabled WRITE setLabelsEnabled NOTIFY labelsEnabledChanged)
    Q_PROPERTY(bool trimEnabled READ trimEnabled WRITE setTrimEnabled NOTIFY trimEnabledChanged)
    Q_PROPERTY(bool directivesEnabled READ directivesEnabled WRITE setDirectivesEnabled NOTIFY directivesEnabledChanged)
    Q_PROPERTY(bool intelEnabled READ intelEnabled WRITE setIntelEnabled NOTIFY intelEnabledChanged)
    Q_PROPERTY(bool commentOnlyEnabled READ commentOnlyEnabled WRITE setCommentOnlyEnabled NOTIFY commentOnlyEnabledChanged)
    Q_PROPERTY(bool demangleEnabled READ demangleEnabled WRITE setDemangleEnabled NOTIFY demangleEnabledChanged)
    Q_PROPERTY(bool rememberLastSession READ rememberLastSession WRITE setRememberLastSession NOTIFY rememberLastSessionChanged)
    Q_PROPERTY(int defaultLanguageIndex READ defaultLanguageIndex WRITE setDefaultLanguageIndex NOTIFY defaultLanguageIndexChanged)
    Q_PROPERTY(int autoRefreshInterval READ autoRefreshInterval WRITE setAutoRefreshInterval NOTIFY autoRefreshIntervalChanged)
    Q_PROPERTY(int editorZoomFactor READ editorZoomFactor WRITE setEditorZoomFactor NOTIFY editorZoomFactorChanged)
    Q_PROPERTY(QStringList examples READ examples NOTIFY examplesChanged)
    Q_PROPERTY(QStringList languages READ languages NOTIFY languagesChanged)
    Q_PROPERTY(QQmlListProperty<Library> libs READ qmlListPropertyLibs NOTIFY libsChanged)
public:
    explicit QuickWidgetAPI(QObject *parent = nullptr);
    Q_INVOKABLE void closeConfiguration();
    Q_INVOKABLE void loadExample(int index);
    Q_INVOKABLE void librarySwitched();

    bool binary() const;
    void setBinary(bool binary);

    bool labels() const;
    void setLabels(bool labels);

    bool trim() const;
    void setTrim(bool trim);

    bool directives() const;
    void setDirectives(bool directives);

    bool intel() const;
    void setIntel(bool intel);

    bool commentOnly() const;
    void setCommentOnly(bool commentOnly);

    bool binaryEnabled() const;
    void setBinaryEnabled(bool binaryEnabled);

    bool labelsEnabled() const;
    void setLabelsEnabled(bool labelsEnabled);

    bool trimEnabled() const;
    void setTrimEnabled(bool trimEnabled);

    bool directivesEnabled() const;
    void setDirectivesEnabled(bool directivesEnabled);

    bool intelEnabled() const;
    void setIntelEnabled(bool intelEnabled);

    bool commentOnlyEnabled() const;
    void setCommentOnlyEnabled(bool commentOnlyEnabled);

    bool demangle() const;
    void setDemangle(bool demangle);

    bool demangleEnabled() const;
    void setDemangleEnabled(bool demangleEnabled);

    const QStringList &examples() const;
    void setExamples(const QStringList &examples);

    void setLibs(LibraryListPtr libs);
    LibraryListPtr libs();
    QQmlListProperty<Library>  qmlListPropertyLibs();

    const QStringList &languages() const;
    void setLanguages(const QStringList &languages);

    bool rememberLastSession() const;
    void setRememberLastSession(bool rememberLastSession);

    int autoRefreshInterval() const;
    void setAutoRefreshInterval(int autoRefreshInterval);

    int editorZoomFactor() const;
    void setEditorZoomFactor(int editorZoomFactor);

    int defaultLanguageIndex() const;
    void setDefaultLanguageIndex(int defaultLanguageIndex);

    bool functionsFromOtherLibraries() const;
    void setFunctionsFromOtherLibraries(bool functionsFromOtherLibraries);
    
    bool functionsFromOtherLibrariesEnabled() const;
    void setFunctionsFromOtherLibrariesEnabled(bool functionsFromOtherLibrariesEnabled);
    
signals:
    void doCloseConfiguration();
    void doLoadExample(const QString& name);
    void doLibrarySwitched();
    
    void binaryChanged();
    void functionsFromOtherLibrariesChanged();
    void labelsChanged();
    void trimChanged();
    void directivesChanged();
    void intelChanged();
    void commentOnlyChanged();
    void demangleChanged();

    void binaryEnabledChanged();
    void functionsFromOtherLibrariesEnabledChanged();
    void labelsEnabledChanged();
    void trimEnabledChanged();
    void directivesEnabledChanged();
    void intelEnabledChanged();
    void commentOnlyEnabledChanged();
    void demangleEnabledChanged();

    void examplesChanged();
    void languagesChanged();
    void libsChanged();
    void rememberLastSessionChanged();
    void defaultLanguageIndexChanged();
    void autoRefreshIntervalChanged();
    void editorZoomFactorChanged();
public slots:

private:
    // code inspector options
    bool m_binary;
    bool m_binaryEnabled;
    bool m_functionsFromOtherLibraries;
    bool m_functionsFromOtherLibrariesEnabled;
    bool m_labels;
    bool m_labelsEnabled;
    bool m_trim;
    bool m_trimEnabled;
    bool m_directives;
    bool m_directivesEnabled;
    bool m_intel;
    bool m_intelEnabled;
    bool m_commentOnly;
    bool m_commentOnlyEnabled;
    bool m_demangle;
    bool m_demangleEnabled;

    QStringList m_examples;
    QStringList m_languages;
    LibraryListPtr m_libs;

    static void appendLib(QQmlListProperty<Library>* list, Library* p);
    static qsizetype libCount(QQmlListProperty<Library> *list);
    static Library  *lib(QQmlListProperty<Library> *list, qsizetype index);
    static void clearLibs(QQmlListProperty<Library>* list);

    void appendLib(Library* p);
    int libCount() const;
    Library *lib(int index) const;
    void clearLibs();
};

typedef QSharedPointer<QuickWidgetAPI> QuickWidgetAPIPtr;
#endif // QUICKWIDGETAPI_H
