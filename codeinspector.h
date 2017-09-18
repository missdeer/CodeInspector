#ifndef CODEINSPECTOR_H
#define CODEINSPECTOR_H

#include "ScintillaEdit.h"
#include "scintillaconfig.h"
#include "godboltagent.h"

class CodeInspector : public ScintillaEdit
{
    Q_OBJECT
public:
    explicit CodeInspector(QWidget *parent = 0);
    void initialize();
    void setContent(const QString& content, bool binary);
    QMap<int, sptr_t> setAsmItems(const QVector<AsmItem>& items, bool binary);
protected:
    virtual QVariant inputMethodQuery(Qt::InputMethodQuery) const;
private slots:
    void linesAdded(int linesAdded);
private:
    ScintillaConfig m_sc;
};

#endif // CODEINSPECTOR_H
