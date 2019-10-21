#include "stdafx.h"

#include "codeinspector.h"

CodeInspector::CodeInspector(QWidget *parent) : ScintillaEdit(parent), m_sc(this) {}

void CodeInspector::initialize()
{
    m_sc.initScintilla();
    m_sc.initInspectorMargins(false);
    m_sc.initLexerStyle("asm");

    connect(this, &ScintillaEdit::marginClicked, this, &CodeInspector::marginClicked);
    setReadOnly(true);
}

void CodeInspector::setContent(const QString &content, bool binary)
{
    auto b = content.toUtf8();
    setReadOnly(false);
    setText(b.data());
    setReadOnly(true);

    emptyUndoBuffer();
    m_sc.initLexerStyle("asm");
    m_sc.initInspectorMargins(binary);
    colourise(0, -1);
}

QMap<int, sptr_t> CodeInspector::setAsmItems(const AsmItemList &items, bool binary)
{
    const sptr_t marginStyleId = STYLE_LASTPREDEFINED + 1;
    sptr_t       textLength    = 0;

    QMap<int, sptr_t> markerMap;
    int               markerIndex = 0;
    m_sc.initMarkers();

    for (int i = 0; i < items.length(); i++)
    {
        const auto &item = items.at(i);

        if (item->source != -1)
        {
            auto it = markerMap.find(item->source);
            if (markerMap.end() == it)
            {
                // pick a color
                markerMap.insert(item->source, markerIndex);
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

        if (binary)
        {
            QString     text;
            QTextStream ts(&text);
            ts.setFieldWidth(2);
            ts.setIntegerBase(16);
            ts.setPadChar('0');
            if (item->address > 0)
            {
                ts << item->address;
            }

            if (!text.isEmpty())
                text.append(":");
            else
                text.append("00:");
            if (!item->opcodes.isEmpty())
            {
                for (const auto &opcode : item->opcodes)
                {
                    ts << opcode;
                }
            }

            // qDebug() << "binary: " << i << text;
            if (text.length() > 3)
            {
                marginSetStyle(i, marginStyleId);
                auto t = text.toUtf8();
                marginSetText(i, t.data());
                auto tl    = textWidth(marginStyleId, t.data());
                textLength = std::max(tl, textLength);
            }
        }
    }

    if (textLength)
        setMarginWidthN(1, textLength);

    if (!binary)
    {
        sptr_t left  = marginLeft() + 2;
        sptr_t right = marginRight() + 2;
        auto   b     = QString::number(lineCount()).toUtf8();
        sptr_t width = left + right + textWidth(STYLE_LINENUMBER, b.data());
        if (width > 32)
            setMarginWidthN(0, width);
    }

#if !defined(QT_NO_DEBUG)
    for (auto it = markerMap.begin(); it != markerMap.end(); ++it)
    {
        qDebug() << "source:" << it.key() << ", marker:" << it.value();
    }
#endif
    return markerMap;
}

QVariant CodeInspector::inputMethodQuery(Qt::InputMethodQuery /*query*/) const
{
    // so that it won't show input method pane on mobile device
    return QVariant(QRectF(0, 0, 0, 0));
}
