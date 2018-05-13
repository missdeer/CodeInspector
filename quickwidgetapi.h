#ifndef QUICKWIDGETAPI_H
#define QUICKWIDGETAPI_H

#include <QObject>

class QuickWidgetAPI : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool binary READ binary WRITE setBinary NOTIFY binaryChanged)
    Q_PROPERTY(bool labels READ labels WRITE setLabels NOTIFY labelsChanged)
    Q_PROPERTY(bool trim READ trim WRITE setTrim NOTIFY trimChanged)
    Q_PROPERTY(bool directives READ directives WRITE setDirectives NOTIFY directivesChanged)
    Q_PROPERTY(bool intel READ intel WRITE setIntel NOTIFY intelChanged)
    Q_PROPERTY(bool commentOnly READ commentOnly WRITE setCommentOnly NOTIFY commentOnlyChanged)
    Q_PROPERTY(bool binaryEnabled READ binaryEnabled WRITE setBinaryEnabled NOTIFY binaryEnabledChanged)
    Q_PROPERTY(bool labelsEnabled READ labelsEnabled WRITE setLabelsEnabled NOTIFY labelsEnabledChanged)
    Q_PROPERTY(bool trimEnabled READ trimEnabled WRITE setTrimEnabled NOTIFY trimEnabledChanged)
    Q_PROPERTY(bool directivesEnabled READ directivesEnabled WRITE setDirectivesEnabled NOTIFY directivesEnabledChanged)
    Q_PROPERTY(bool intelEnabled READ intelEnabled WRITE setIntelEnabled NOTIFY intelEnabledChanged)
    Q_PROPERTY(bool commentOnlyEnabled READ commentOnlyEnabled WRITE setCommentOnlyEnabled NOTIFY commentOnlyEnabledChanged)
public:
    explicit QuickWidgetAPI(QObject *parent = nullptr);

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

signals:
    void binaryChanged();
    void labelsChanged();
    void trimChanged();
    void directivesChanged();
    void intelChanged();
    void commentOnlyChanged();
    void binaryEnabledChanged();
    void labelsEnabledChanged();
    void trimEnabledChanged();
    void directivesEnabledChanged();
    void intelEnabledChanged();
    void commentOnlyEnabledChanged();
public slots:

private:
    // code inspector options
    bool m_binary;
    bool m_binaryEnabled;
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
};

typedef QSharedPointer<QuickWidgetAPI> QuickWidgetAPIPtr;
#endif // QUICKWIDGETAPI_H
