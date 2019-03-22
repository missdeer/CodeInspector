#ifndef CODEINSPECTOR_H
#define CODEINSPECTOR_H

#include "ScintillaEdit.h"
#include "scintillaconfig.h"
#include "godboltagent.h"

class CodeInspector : public ScintillaEdit
{
    Q_OBJECT
public:
    explicit CodeInspector(QWidget *parent = nullptr);
    void initialize();
    void setContent(const QString& content, bool binary);
    QMap<int, sptr_t> setAsmItems(const AsmItemList &items, bool binary);
protected:

private:
    ScintillaConfig m_sc;
};

#endif // CODEINSPECTOR_H
