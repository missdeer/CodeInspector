#ifndef CODEINSPECTOR_H
#define CODEINSPECTOR_H

#include <QMap>

#include "ScintillaEdit.h"
#include "godboltagent.h"
#include "scintillaconfig.h"

class CodeInspector : public ScintillaEdit
{
    Q_OBJECT
public:
    explicit CodeInspector(QWidget *parent = nullptr);
    void initialize();
    void setContent(const QString &content, bool binary);
    void setAsmItems(const AsmItemList &items, bool binary, QMap<int, intptr_t> &markerMap);

protected:
    QVariant inputMethodQuery(Qt::InputMethodQuery) const override;

private:
    ScintillaConfig m_sc;
};

#endif // CODEINSPECTOR_H
