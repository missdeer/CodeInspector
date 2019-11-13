#include "stdafx.h"

#include "llvmirview.h"

LLVMIRView::LLVMIRView(QWidget *parent) : ScintillaEdit(parent), m_sc(this) {}

void LLVMIRView::initialize()
{
    m_sc.initScintilla();
    m_sc.initInspectorMargins(false);
    m_sc.initLexerStyle("asm");

    setReadOnly(true);
}

void LLVMIRView::setContent(const QString &content)
{
    auto b = content.toUtf8();
    setReadOnly(false);
    setText(b.data());
    setReadOnly(true);

    emptyUndoBuffer();
    m_sc.initLexerStyle("asm");
    colourise(0, -1);
}

void LLVMIRView::setLLVMIRItems(const LLVMIRItemList &items, QMap<int, intptr_t> &markerMap)
{
    intptr_t markerIndex = 0;
    m_sc.initMarkers();

    for (int i = 0; i < items.length(); i++)
    {
        const auto &item = items.at(i);

        if (item->sourceLine != -1)
        {
            auto it = markerMap.find(item->sourceLine);
            if (markerMap.end() == it)
            {
                // pick a color
                markerMap.insert(item->sourceLine, markerIndex);
                markerAdd(i, markerIndex);

                markerIndex++;
                if (markerIndex == 12)
                    markerIndex = 0;
            }
            else
            {
                markerAdd(i, it.value());
            }
        }
    }
}

QVariant LLVMIRView::inputMethodQuery(Qt::InputMethodQuery /*query*/) const
{
    // so that it won't show input method pane on mobile device
    return QVariant(QRectF(0, 0, 0, 0));
}
