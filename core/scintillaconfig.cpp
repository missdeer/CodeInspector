#include "stdafx.h"
#include "settings.h"
#include "ScintillaEdit.h"
#include "scintillaconfig.h"

void ScintillaConfig::initScintilla()
{
    m_sci->styleResetDefault();
    m_sci->styleClearAll();
    m_sci->clearDocumentStyle();
    m_sci->setHScrollBar(true);
    m_sci->setVScrollBar(true);
    m_sci->setXOffset(0);
    m_sci->setScrollWidth(1);
    m_sci->setScrollWidthTracking(true);
    m_sci->setEndAtLastLine(true);
    m_sci->setWhitespaceFore(true, 0x808080);
    m_sci->setWhitespaceBack(true, 0xFFFFFF);
    m_sci->setMouseDownCaptures(true);
#if defined(Q_OS_WIN)
    m_sci->setEOLMode(SC_EOL_CRLF);
#else
    m_sci->setEOLMode(SC_EOL_LF);
#endif
    m_sci->setViewEOL(false);
    m_sci->setViewWS(SCWS_INVISIBLE);
    m_sci->setCaretFore(0x0000FF);
    //m_sci->setCaretLineVisible(true);
    //m_sci->setCaretLineBack(0xFFFFD0);
    //m_sci->setCaretLineBackAlpha(256);
    m_sci->setCaretPeriod(500);
    m_sci->setCaretWidth(2);
    m_sci->setCaretSticky(0);
    m_sci->setSelFore(true, 0xFFFFFF);
    m_sci->setSelBack(true, 0xC56A31);
    m_sci->setSelAlpha(256);
    m_sci->setSelEOLFilled(true);
    m_sci->setAdditionalSelectionTyping(true);
    m_sci->setVirtualSpaceOptions(SCVS_RECTANGULARSELECTION);
    m_sci->setHotspotActiveFore(true, 0x0000FF);
    m_sci->setHotspotActiveBack(true, 0xFFFFFF);
    m_sci->setHotspotActiveUnderline(true);
    m_sci->setHotspotSingleLine(false);
    m_sci->setControlCharSymbol(0);

    m_sci->setTabWidth(4);
    m_sci->setUseTabs(false);
    m_sci->setIndent(4);
    m_sci->setTabIndents(false);
    m_sci->setBackSpaceUnIndents(false);
    m_sci->setIndentationGuides( SC_IV_REAL);
    m_sci->setHighlightGuide(1);
    m_sci->setPrintMagnification(1);
    m_sci->setPrintColourMode(0);
    m_sci->setPrintWrapMode(1);

    m_sci->setWrapMode(SC_WRAP_NONE);
    m_sci->setWrapVisualFlags(SC_WRAPVISUALFLAG_END);
    m_sci->setWrapVisualFlagsLocation(SC_WRAPVISUALFLAGLOC_DEFAULT);
    m_sci->setWrapStartIndent(0);

    m_sci->setLayoutCache(SC_CACHE_PAGE);
    m_sci->linesSplit(0);
    m_sci->setEdgeMode(0);
    m_sci->setEdgeColumn(200);
    m_sci->setEdgeColour(0xC0DCC0);

    m_sci->usePopUp(true);

#if defined(Q_OS_WIN)
    m_sci->setTechnology(SC_TECHNOLOGY_DIRECTWRITEDC );
#endif
    m_sci->setBufferedDraw(false);
    m_sci->setPhasesDraw(SC_PHASES_TWO);
    m_sci->setCodePage(SC_CP_UTF8);
    m_sci->setWordChars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_");
    m_sci->setZoom(1);
    m_sci->setWhitespaceChars(NULL);
    m_sci->setMouseDwellTime(2500);

    m_sci->setSavePoint();
    m_sci->setFontQuality( SC_EFF_QUALITY_ANTIALIASED);

    // apply global settings
    QString themePath = ":/resource/sci/themes/" % g_settings->codeEditorTheme() % ".xml";
    if (!QFile::exists(themePath))
        themePath = ":/resource/sci/stylers.model.xml";
    applyThemeStyle(themePath, "cpp");
}

void ScintillaConfig::initEditorFolderStyle()
{
    m_sci->setFoldFlags(SC_FOLDFLAG_LINEAFTER_CONTRACTED );

    m_sci->markerDefine(SC_MARKNUM_FOLDEROPEN, SC_MARK_BOXMINUS);
    m_sci->markerDefine(SC_MARKNUM_FOLDER, SC_MARK_BOXPLUS);
    m_sci->markerDefine(SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE);
    m_sci->markerDefine(SC_MARKNUM_FOLDERTAIL, SC_MARK_LCORNER);
    m_sci->markerDefine(SC_MARKNUM_FOLDEREND, SC_MARK_BOXPLUSCONNECTED);
    m_sci->markerDefine(SC_MARKNUM_FOLDEROPENMID, SC_MARK_BOXMINUSCONNECTED);
    m_sci->markerDefine(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_TCORNER);

    m_sci->markerSetFore(SC_MARKNUM_FOLDEROPEN, 0xFFFFFF);
    m_sci->markerSetBack(SC_MARKNUM_FOLDEROPEN, 0x808080);
    m_sci->markerSetFore(SC_MARKNUM_FOLDER, 0xFFFFFF);
    m_sci->markerSetBack(SC_MARKNUM_FOLDER, 0x808080);
    m_sci->markerSetFore(SC_MARKNUM_FOLDERSUB, 0xFFFFFF);
    m_sci->markerSetBack(SC_MARKNUM_FOLDERSUB, 0x808080);
    m_sci->markerSetFore(SC_MARKNUM_FOLDERTAIL, 0xFFFFFF);
    m_sci->markerSetBack(SC_MARKNUM_FOLDERTAIL, 0x808080);
    m_sci->markerSetFore(SC_MARKNUM_FOLDEREND, 0xFFFFFF);
    m_sci->markerSetBack(SC_MARKNUM_FOLDEREND, 0x808080);
    m_sci->markerSetFore(SC_MARKNUM_FOLDEROPENMID, 0xFFFFFF);
    m_sci->markerSetBack(SC_MARKNUM_FOLDEROPENMID, 0x808080);
    m_sci->markerSetFore(SC_MARKNUM_FOLDERMIDTAIL, 0xFFFFFF);
    m_sci->markerSetBack(SC_MARKNUM_FOLDERMIDTAIL, 0x808080);
    m_sci->setProperty( "fold", "1");
    m_sci->setProperty( "fold.flags", "16");
    m_sci->setProperty( "fold.symbols", "1");
    m_sci->setProperty( "fold.compact", "0");
    m_sci->setProperty( "fold.at.else", "1");
    m_sci->setProperty( "fold.preprocessor", "1");
    m_sci->setProperty( "fold.view", "1");
    m_sci->setProperty( "fold.comment", "1");
    m_sci->setProperty( "fold.html", "1");
    m_sci->setProperty( "fold.comment.python", "1");
    m_sci->setProperty( "fold.quotes.python", "1");
}

void ScintillaConfig::initLexerStyle(const QString& lang)
{
    // apply language specified settings
    QString themePath = ":/resource/sci/themes/" % g_settings->codeEditorTheme() % ".xml";
    if (!QFile::exists(themePath))
        themePath = ":/resource/sci/stylers.model.xml";
    applyThemeStyle(themePath, lang);

    // read configurations from langs.model.xml
    QString configPath = ":/resource/sci/langs.model.xml";
    applyLanguageStyle(configPath, lang);
}

void ScintillaConfig::initEditorMargins()
{
    m_sci->setMarginLeft(4);
    //sci->setMarginRight(4);
    m_sci->setMarginTypeN(0, SC_MARGIN_NUMBER);
    m_sci->setMarginWidthN(0, 32);
    m_sci->setMarginMaskN(0, 0);
    m_sci->setMarginSensitiveN(0, false);
    m_sci->setMarginTypeN(1, SC_MARGIN_SYMBOL);
    m_sci->setMarginWidthN(1, 0);
    m_sci->setMarginMaskN(1, ~SC_MASK_FOLDERS); //~SC_MASK_FOLDERS or 0x1FFFFFF or 33554431
    m_sci->setMarginSensitiveN(1, true);
    m_sci->setMarginTypeN(2, SC_MARGIN_SYMBOL);
    m_sci->setMarginWidthN(2, 16);
    m_sci->setMarginMaskN(2, SC_MASK_FOLDERS);// 0xFE000000 or -33554432
    m_sci->setMarginSensitiveN(2, true);

    m_sci->setFoldMarginColour(true, 0xE9E9E9);
    m_sci->setFoldMarginHiColour(true, 0xFFFFFF);
}

void ScintillaConfig::initInspectorMargins(bool binary)
{
    m_sci->setMarginLeft(4);
    //sci->setMarginRight(4);
    m_sci->setMarginTypeN(0, SC_MARGIN_NUMBER);
    m_sci->setMarginWidthN(0, binary ? 0 : 32);
    m_sci->setMarginMaskN(0, 0);
    m_sci->setMarginSensitiveN(0, false);

    m_sci->setMarginTypeN(1, SC_MARGIN_TEXT);
    m_sci->setMarginWidthN(1, binary ? 32 : 0);
    m_sci->setMarginMaskN(1, ~SC_MASK_FOLDERS); //~SC_MASK_FOLDERS or 0x1FFFFFF or 33554431
    m_sci->setMarginSensitiveN(1, false);
}

void ScintillaConfig::inttOuputWindowMargins()
{
    m_sci->setMarginWidthN(0, 0);
    m_sci->setMarginWidthN(1, 0);
    m_sci->setMarginWidthN(2, 0);
}

void ScintillaConfig::initMarkers()
{
    const int lineBackgroundColorCount = 12;
    sptr_t lineBackgroundColors[lineBackgroundColorCount] = {
        0xD3E4EF,
        0xFEE5C8,
        0xE8E7F1,
        0xD7F0EB,
        0xFEEEF5,
        0xEDF8EA,
        0xE4F3CA,
        0xFFF9CC,
        0xF2F2F1,
        0xE8D3E7,
        0xFFFFE4,
        0xFED3CD,
    };

    for ( size_t i = 0; i < lineBackgroundColorCount; i++)
    {
        m_sci->markerDefine(static_cast<sptr_t>(i), SC_MARK_BACKGROUND);
        m_sci->markerSetBack(static_cast<sptr_t>(i), lineBackgroundColors[i]);
        m_sci->markerDeleteAll(static_cast<sptr_t>(i));
    }
}

void ScintillaConfig::applyLanguageStyle(const QString &configPath, const QString& lang)
{
    QDomDocument doc;
    QFile file(configPath);
    if (!file.open(QIODevice::ReadOnly))
        return;
    QString errMsg;
    int errLine;
    if (!doc.setContent(&file, &errMsg, &errLine))
    {
        qDebug() << "parsing xml document failed:" << configPath << errMsg << errLine;
        file.close();
        return;
    }
    file.close();

    QDomElement docElem = doc.documentElement();
    QDomElement languagesElem = docElem.firstChildElement("Languages");

    QDomElement langElem = languagesElem.firstChildElement("Language");
    while (!langElem.isNull() && langElem.attribute("name") != lang)
        langElem = langElem.nextSiblingElement("Language");

    if (langElem.isNull())
        return;

    QString commentLine = langElem.attribute("commentLine");
    QString commentStart = langElem.attribute("commentStart");
    QString commentEnd = langElem.attribute("commentEnd");
    QString lexer = langElem.attribute("lexer");
    if (lexer.isEmpty())
        lexer = lang;

    QDomElement keywordElem = langElem.firstChildElement("Keywords");
    int keywordSet = 0;
    while(!keywordElem.isNull())
    {
        QString keyword = keywordElem.text();
        m_sci->setKeyWords(keywordSet++, keyword.toStdString().c_str());
        keywordElem = keywordElem.nextSiblingElement("Keywords");
    }

    auto l = lexer.toUtf8();
    m_sci->setLexerLanguage(l.data());
}

void ScintillaConfig::applyThemeStyle(const QString &themePath, const QString& lang)
{
    QDomDocument doc;
    QFile file(themePath);
    if (!file.open(QIODevice::ReadOnly))
        return;
    QString errMsg;
    int errLine;
    if (!doc.setContent(&file, &errMsg, &errLine))
    {
        qDebug() << "parsing xml document failed:" << themePath << errMsg << errLine;
        file.close();
        return;
    }
    file.close();

    QDomElement docElem = doc.documentElement();

    QDomElement globalStylesElem = docElem.firstChildElement("GlobalStyles");

    for(QDomElement styleElem = globalStylesElem.firstChildElement("WidgetStyle");
        !styleElem.isNull();
        styleElem = styleElem.nextSiblingElement("WidgetStyle"))
    {
        applyStyle(styleElem);
    }

    QDomElement lexerStylesElem = docElem.firstChildElement("LexerStyles");
    QDomElement lexerTypeElem = lexerStylesElem.firstChildElement("LexerType");
    while (!lexerTypeElem.isNull() && lexerTypeElem.attribute("name") != lang)
        lexerTypeElem = lexerTypeElem.nextSiblingElement("LexerType");

    if (lexerTypeElem.isNull())
        return;

    for (QDomElement styleElem = lexerTypeElem.firstChildElement("WordsStyle");
         !styleElem.isNull();
         styleElem = styleElem.nextSiblingElement("WordsStyle"))
    {
        applyStyle(styleElem);
    }
}

void ScintillaConfig::applyStyle(const QDomElement &styleElem)
{
    int id = styleElem.attribute("styleID").toInt();
    if (id != 0)
    {
        QString foreColor = styleElem.attribute("fgColor");
        if (!foreColor.isEmpty())
        {
            int color = foreColor.toLong(NULL, 16);
            color = ((color & 0xFF0000) >> 16) | (color & 0xFF00) | ((color & 0xFF) << 16);
            m_sci->styleSetFore(id, color);
        }
        QString backColor = styleElem.attribute("bgColor");
        if (!backColor.isEmpty())
        {
            int color = backColor.toLong(NULL, 16);
            color = ((color & 0xFF0000) >> 16) | (color & 0xFF00) | ((color & 0xFF) << 16);
            m_sci->styleSetBack(id, color);
        }
    }
    else if (styleElem.attribute("name") != "Global override")
    {
        return;
    }

    QString fontName = styleElem.attribute("fontName");
    QFontDatabase database;
    QStringList families = database.families();
    if (!fontName.isEmpty() && families.contains(fontName))
        m_sci->styleSetFont(id, fontName.toStdString().c_str());
    else
    {
        if (families.contains(g_settings->codeEditorFontFamily()))
            m_sci->styleSetFont(id, g_settings->codeEditorFontFamily().toStdString().c_str());
        else
#if defined(Q_OS_MAC) || defined (Q_OS_IOS)
            m_sci->styleSetFont(id, "Menlo");
#elif defined(Q_OS_WIN)
            m_sci->styleSetFont(id, "Consolas");
#elif defined(Q_OS_ANDROID)
            m_sci->styleSetFont(id, "Droid Sans Mono");
#else
            m_sci->styleSetFont(id, "Monospace");
#endif
    }

    uint fontStyle = styleElem.attribute("fontStyle").toUInt();
    if (fontStyle & 0x01)
        m_sci->styleSetBold(id, true);
    if (fontStyle & 0x02)
        m_sci->styleSetItalic(id, true);
    if (fontStyle & 0x04)
        m_sci->styleSetUnderline(id, true);
    if (fontStyle & 0x08)
        m_sci->styleSetVisible(id, true);
    if (fontStyle & 0x10)
        m_sci->styleSetCase(id, true);
    if (fontStyle & 0x20)
        m_sci->styleSetEOLFilled(id, true);
    if (fontStyle & 0x40)
        m_sci->styleSetHotSpot(id, true);
    if (fontStyle & 0x80)
        m_sci->styleSetChangeable(id, true);
    QString fontSize = styleElem.attribute("fontSize");
    if (!fontSize.isEmpty())
        m_sci->styleSetSize(id, std::max(12, fontSize.toInt()));
    else
        m_sci->styleSetSize(id, 12);
}