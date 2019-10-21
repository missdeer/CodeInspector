//
// Created by Fan Yang on 2019/10/21.
//

#ifndef CODEINSPECTOR_LLVMIRVIEW_H
#define CODEINSPECTOR_LLVMIRVIEW_H

#include "ScintillaEdit.h"
#include "godboltagent.h"
#include "scintillaconfig.h"

class LLVMIRView : public ScintillaEdit
{
    Q_OBJECT
  public:
    explicit LLVMIRView(QWidget *parent = nullptr);
    void initialize();
    void setContent(const QString &content);

  protected:
    QVariant inputMethodQuery(Qt::InputMethodQuery) const override;

  private:
    ScintillaConfig m_sc;
};

#endif // CODEINSPECTOR_LLVMIRVIEW_H
