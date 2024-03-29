#include <functional>

#include <QDateTime>
#include <QDir>
#include <QDomDocument>
#include <QFile>
#include <QFontDatabase>
#include <QStandardPaths>
#include <QStringBuilder>

#include "scintillaconfig.h"
#include "ILexer.h"
#include "Lexilla.h"
#include "ScintillaEdit.h"
#include "Scintillua.h"
#include "settings.h"

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
    // m_sci->setCaretLineVisible(true);
    // m_sci->setCaretLineBack(0xFFFFD0);
    // m_sci->setCaretLineBackAlpha(256);
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
    m_sci->setIndentationGuides(SC_IV_REAL);
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
    m_sci->setTechnology(SC_TECHNOLOGY_DIRECTWRITEDC);
#endif
    m_sci->setBufferedDraw(false);
    m_sci->setPhasesDraw(SC_PHASES_TWO);
    m_sci->setCodePage(SC_CP_UTF8);
    m_sci->setWordChars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_");
    m_sci->setZoom(1);
    m_sci->setWhitespaceChars(nullptr);
    m_sci->setMouseDwellTime(2500);

    m_sci->setSavePoint();
    m_sci->setFontQuality(SC_EFF_QUALITY_ANTIALIASED);

    // apply global settings
    QString themePath = ":/resource/sci/themes/" % g_settings->codeEditorTheme() % ".xml";
    if (!QFile::exists(themePath))
    {
        themePath = ":/resource/sci/themes/Default.xml";
    }
    applyThemeStyle(themePath, QStringLiteral("cpp"));
}

void ScintillaConfig::initEditorFolderStyle()
{
    m_sci->setFoldFlags(SC_FOLDFLAG_LINEAFTER_CONTRACTED);

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
    m_sci->setProperty("fold", "1");
    m_sci->setProperty("fold.flags", "16");
    m_sci->setProperty("fold.symbols", "1");
    m_sci->setProperty("fold.compact", "0");
    m_sci->setProperty("fold.at.else", "1");
    m_sci->setProperty("fold.preprocessor", "1");
    m_sci->setProperty("fold.view", "1");
    m_sci->setProperty("fold.comment", "1");
    m_sci->setProperty("fold.html", "1");
    m_sci->setProperty("fold.comment.python", "1");
    m_sci->setProperty("fold.quotes.python", "1");
}

void ScintillaConfig::initLexerStyle(const QString &lang)
{
    QString lexer = lang.toLower();
    // apply language specified settings
    QString themePath = ":/resource/sci/themes/" % g_settings->codeEditorTheme() % ".xml";
    if (!QFile::exists(themePath))
    {
        themePath = ":/resource/sci/themes/Default.xml";
    }
    applyThemeStyle(themePath, lexer);

    // read configurations from langs.model.xml
    const QString configPath = ":/resource/sci/langs.model.xml";

    applyLexer(configPath, lexer);
}

void ScintillaConfig::initEditorMargins()
{
    m_sci->setMarginLeft(4);
    // sci->setMarginRight(4);
    m_sci->setMarginTypeN(0, SC_MARGIN_NUMBER);
    m_sci->setMarginWidthN(0, 32);
    m_sci->setMarginMaskN(0, 0);
    m_sci->setMarginSensitiveN(0, false);
    m_sci->setMarginTypeN(1, SC_MARGIN_SYMBOL);
    m_sci->setMarginWidthN(1, 0);
    m_sci->setMarginMaskN(1, ~SC_MASK_FOLDERS); //~SC_MASK_FOLDERS or 0x1FFFFFF or 33554431
    m_sci->setMarginSensitiveN(1, true);
    m_sci->setMarginTypeN(2, SC_MARGIN_SYMBOL);
    m_sci->setMarginWidthN(2, 12);
    m_sci->setMarginMaskN(2, SC_MASK_FOLDERS); // 0xFE000000 or -33554432
    m_sci->setMarginSensitiveN(2, true);

    m_sci->setFoldMarginColour(true, 0xE9E9E9);
    m_sci->setFoldMarginHiColour(true, 0xFFFFFF);
}

void ScintillaConfig::initInspectorMargins(bool binary)
{
    m_sci->setMarginLeft(4);
    // sci->setMarginRight(4);
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
    sptr_t lineBackgroundColors[] = {
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

    for (size_t i = 0; i < sizeof(lineBackgroundColors) / sizeof(lineBackgroundColors[0]); i++)
    {
        m_sci->markerDefine(static_cast<sptr_t>(i), SC_MARK_BACKGROUND);
        m_sci->markerSetBack(static_cast<sptr_t>(i), lineBackgroundColors[i]);
        m_sci->markerDeleteAll(static_cast<sptr_t>(i));
    }
}

void ScintillaConfig::applyLexer(const QString &configPath, const QString &lang)
{
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO << __LINE__ << configPath << lang;
#endif
    QString lexer = lang.toLower();

    auto psci = m_sci->directPointer();
    m_sci->privateLexerCall(SCI_SETDOCPOINTER, psci);

    QMap<QString, QString> langMap = {
        {QStringLiteral("c++"), QStringLiteral("cpp")},
        {QStringLiteral("assembly"), QStringLiteral("asm")},
        {QStringLiteral("analysis"), QStringLiteral("asm")},
        {QStringLiteral("llvm ir"), QStringLiteral("llvm")},
    };
    if (langMap.contains(lexer))
    {
        lexer = langMap.value(lexer);
    }
#if defined(LOGS_ENABLED)
    qDebug() << Q_FUNC_INFO << __LINE__ << lexer;
#endif
    void *lexerId = CreateLexer(lexer.toStdString().c_str());
    if (!lexerId)
    {
        m_sci->setILexer((sptr_t)ScintilluaNS::CreateLexer(nullptr));

        auto lexersPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/lexers";
        QDir dir(lexersPath);
        if (!dir.exists(lexersPath + "/themes"))
        {
            dir.mkpath(lexersPath + "/themes");
        }
        extractScintilluaLexers(lexersPath);

        ScintilluaNS::SetLibraryProperty("lpeg.home", QDir::toNativeSeparators(lexersPath).toUtf8().data());
        ScintilluaNS::SetLibraryProperty("lpeg.color.theme", "scite");

        langMap.insert({{QStringLiteral("c"), QStringLiteral("ansi_c")},
                        {QStringLiteral("d"), QStringLiteral("dmd")},
                        {QStringLiteral("ocaml"), QStringLiteral("caml")}});
        if (langMap.contains(lexer))
        {
            lexer = langMap.value(lexer);
        }

        lexerId = ScintilluaNS::CreateLexer(lexer.toStdString().c_str());
#if defined(LOGS_ENABLED)
        qDebug() << Q_FUNC_INFO << __LINE__ << "scintillua lexer:" << lexer << lexerId;
#endif
    }
    if (!lexerId)
    {
        lexer   = "cpp";
        lexerId = CreateLexer(lexer.toStdString().c_str());
#if defined(LOGS_ENABLED)
        qDebug() << Q_FUNC_INFO << __LINE__ << "fallback to lexilla cpp lexer";
#endif
    }
    m_sci->setILexer((sptr_t)lexerId);

    QDomDocument doc;
    QFile        file(configPath);
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QString errMsg;
    int     errLine   = 0;
    int     errColumn = 0;
    if (!doc.setContent(&file, &errMsg, &errLine, &errColumn))
    {
#if defined(LOGS_ENABLED)
        qDebug() << "parsing xml document failed:" << configPath << errMsg << errLine << errColumn;
#endif
        return;
    }

    QDomElement docElem       = doc.documentElement();
    QDomElement languagesElem = docElem.firstChildElement(QStringLiteral("Languages"));

    QDomElement langElem = languagesElem.firstChildElement(QStringLiteral("Language"));
    while (!langElem.isNull() && langElem.attribute(QStringLiteral("name")) != lexer && langElem.attribute(QStringLiteral("name")) != lang.toLower())
    {
        langElem = langElem.nextSiblingElement(QStringLiteral("Language"));
    }
    QDomElement keywordElem = langElem.firstChildElement(QStringLiteral("Keywords"));
    int         keywordSet  = 0;
    while (!keywordElem.isNull())
    {
        QString keyword = keywordElem.text();
#if defined(LOGS_ENABLED)
        qDebug() << keywordSet << keywordElem.attribute(QStringLiteral("name")) << keyword;
#endif
        m_sci->setKeyWords(keywordSet++, keyword.toStdString().c_str());
        keywordElem = keywordElem.nextSiblingElement(QStringLiteral("Keywords"));
    }
}

void ScintillaConfig::applyThemeStyle(const QString &themePath, const QString &lang)
{
    QDomDocument doc;
    QFile        file(themePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QString errMsg;
    int     errLine   = 0;
    int     errColumn = 0;
    if (!doc.setContent(&file, &errMsg, &errLine, &errColumn))
    {
#if defined(LOGS_ENABLED)
        qDebug() << "parsing xml document failed:" << themePath << errMsg << errLine << errColumn;
#endif
        return;
    }

    QDomElement docElem = doc.documentElement();

    QDomElement globalStylesElem = docElem.firstChildElement(QStringLiteral("GlobalStyles"));

    for (QDomElement styleElem = globalStylesElem.firstChildElement(QStringLiteral("WidgetStyle")); !styleElem.isNull();
         styleElem             = styleElem.nextSiblingElement(QStringLiteral("WidgetStyle")))
    {
        if (styleElem.attribute(QStringLiteral("styleID")).toInt() == 0)
        {
            applyGlobalStyle(styleElem);
        }
        else
        {
            applyStyle(styleElem);
        }
    }

    QDomElement lexerStylesElem = docElem.firstChildElement(QStringLiteral("LexerStyles"));
    QDomElement lexerTypeElem   = lexerStylesElem.firstChildElement(QStringLiteral("LexerType"));
    while (!lexerTypeElem.isNull() && lexerTypeElem.attribute(QStringLiteral("name")) != lang)
    {
        lexerTypeElem = lexerTypeElem.nextSiblingElement(QStringLiteral("LexerType"));
    }

    if (lexerTypeElem.isNull())
    {
        return;
    }

    for (QDomElement styleElem = lexerTypeElem.firstChildElement(QStringLiteral("WordsStyle")); !styleElem.isNull();
         styleElem             = styleElem.nextSiblingElement(QStringLiteral("WordsStyle")))
    {
        applyStyle(styleElem);
    }
}

void ScintillaConfig::extractScintilluaLexers(const QString &lexersPath)
{
    QDir resDir(":/scintillua/lexers");
    auto resEntryInfos = resDir.entryInfoList();
    for (const auto &info : qAsConst(resEntryInfos))
    {
        QString localFileName = lexersPath + "/" + info.fileName();
        if (!QFile::exists(localFileName) || QFileInfo(localFileName).lastModified() < info.lastModified())
        {
            QFile from(info.filePath());
            QFile to(localFileName);
            if (from.open(QIODevice::ReadOnly) && to.open(QIODevice::WriteOnly | QIODevice::Truncate))
            {
                to.write(from.readAll());
            }
        }
    }
    resDir.cd(QStringLiteral("themes"));
    resEntryInfos = resDir.entryInfoList();
    for (const auto &info : qAsConst(resEntryInfos))
    {
        QString localFileName = lexersPath + "/themes/" + info.fileName();
        if (!QFile::exists(localFileName) || QFileInfo(localFileName).lastModified() < info.lastModified())
        {
            QFile fromFile(info.filePath());
            QFile toFile(localFileName);
            if (fromFile.open(QIODevice::ReadOnly) && toFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
            {
                toFile.write(fromFile.readAll());
            }
        }
    }
}

void ScintillaConfig::applyStyle(const QDomElement &styleElem)
{
    int styleId = styleElem.attribute(QStringLiteral("styleID")).toInt();
    m_sci->styleSetFore(styleId, 0x0);
    if (styleElem.hasAttribute(QStringLiteral("fgColor")))
    {
        QString foreColor = styleElem.attribute(QStringLiteral("fgColor"));
        if (!foreColor.isEmpty())
        {
            int color = foreColor.toLong(nullptr, 16);
            color     = ((color & 0xFF0000) >> 16) | (color & 0xFF00) | ((color & 0xFF) << 16);
            m_sci->styleSetFore(styleId, color);
        }
    }

    m_sci->styleSetBack(styleId, 0xFFFFFF);
    if (styleId == 33 && styleElem.hasAttribute(QStringLiteral("bgColor"))) // line number margin
    {
        QString backColor = styleElem.attribute(QStringLiteral("bgColor"));
        if (!backColor.isEmpty())
        {
            int color = backColor.toLong(nullptr, 16);
            color     = ((color & 0xFF0000) >> 16) | (color & 0xFF00) | ((color & 0xFF) << 16);
            m_sci->styleSetBack(styleId, color);
        }
    }

    QString fontName = styleElem.attribute(QStringLiteral("fontName"));
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QStringList families = QFontDatabase::families();
#else
    QFontDatabase db;
    QStringList   families = db.families();
#endif

    QStringList fonts = g_settings->codeEditorFontFamily().split(";");
#if defined(Q_OS_MAC) || defined(Q_OS_IOS)
    fonts.append(QStringLiteral("Monaco"));
#elif defined(Q_OS_WIN)
    fonts.append(QStringLiteral("Courier New"));
#elif defined(Q_OS_ANDROID)
    fonts.append(QStringLiteral("Droid Sans Mono"));
#else
    fonts.append(QStringLiteral("Monospace"));
#endif
    for (const auto &f : fonts)
    {
        if (families.contains(f))
        {
            m_sci->styleSetFont(styleId, f.toStdString().c_str());
            break;
        }
    }

    if (styleElem.hasAttribute(QStringLiteral("fontName")))
    {
        if (styleId != 0 && styleId != 32 && !fontName.isEmpty() && families.contains(fontName))
        {
            m_sci->styleSetFont(styleId, fontName.toStdString().c_str());
        }
    }

    if (styleElem.hasAttribute(QStringLiteral("fontStyle")))
    {
        uint fontStyle = styleElem.attribute(QStringLiteral("fontStyle")).toUInt();

        static std::map<unsigned char, std::function<void(sptr_t, bool)>> styleSetterMap = {
            {0x01, [this](sptr_t styleId, bool value) { m_sci->styleSetBold(styleId, value); }},
            {0x02, [this](sptr_t styleId, bool value) { m_sci->styleSetItalic(styleId, value); }},
            {0x04, [this](sptr_t styleId, bool value) { m_sci->styleSetUnderline(styleId, value); }},
            {0x08, [this](sptr_t styleId, bool value) { m_sci->styleSetVisible(styleId, value); }},
            {0x10, [this](sptr_t styleId, bool value) { m_sci->styleSetCase(styleId, value); }},
            {0x20, [this](sptr_t styleId, bool value) { m_sci->styleSetEOLFilled(styleId, value); }},
            {0x40, [this](sptr_t styleId, bool value) { m_sci->styleSetHotSpot(styleId, value); }},
            {0x80, [this](sptr_t styleId, bool value) { m_sci->styleSetChangeable(styleId, value); }},
        };
        for (auto &[key, setter] : styleSetterMap)
        {
            if (fontStyle & key)
            {
                setter(styleId, true);
            }
        }
    }

#if defined(Q_OS_WIN)
    const int defaultFontSize = 12;
#else
    const int defaultFontSize = 14;
#endif
    m_sci->styleSetSize(styleId, defaultFontSize);
    if (styleElem.hasAttribute(QStringLiteral("fontSize")))
    {
        QString fontSize = styleElem.attribute(QStringLiteral("fontSize"));
        if (!fontSize.isEmpty())
        {
            m_sci->styleSetSize(styleId, std::max(defaultFontSize, fontSize.toInt()));
        }
    }
}

void ScintillaConfig::applyGlobalStyle(const QDomElement &styleElem)
{
    QString name = styleElem.attribute(QStringLiteral("name"));
    if (name == QStringLiteral("Global override"))
    {
        applyStyle(styleElem);
    }
    //    if (name == "Current line background colour")
    //    {
    //        QString backColor = styleElem.attribute("bgColor");
    //        if (!backColor.isEmpty())
    //        {
    //            int color = backColor.toLong(nullptr, 16);
    //            color     = ((color & 0xFF0000) >> 16) | (color & 0xFF00) | ((color & 0xFF) << 16);
    //            m_sci->setCaretLineBack(color);
    //        }
    //    }
    if (name == QStringLiteral("Selected text colour"))
    {
        int     color     = 0x808080;
        QString backColor = styleElem.attribute(QStringLiteral("bgColor"));
        if (!backColor.isEmpty())
        {
            color = backColor.toLong(nullptr, 16);
            color = ((color & 0xFF0000) >> 16) | (color & 0xFF00) | ((color & 0xFF) << 16);
        }
        m_sci->setSelBack(true, color);

        color             = 0;
        QString foreColor = styleElem.attribute(QStringLiteral("fgColor"));
        if (!foreColor.isEmpty())
        {
            color = foreColor.toLong(nullptr, 16);
            color = ((color & 0xFF0000) >> 16) | (color & 0xFF00) | ((color & 0xFF) << 16);
        }
        m_sci->setSelFore(true, color);
    }
    if (name == QStringLiteral("Edge colour"))
    {
        int     color     = 0xc0c0c0;
        QString foreColor = styleElem.attribute(QStringLiteral("fgColor"));
        if (!foreColor.isEmpty())
        {
            color = foreColor.toLong(nullptr, 16);
            color = ((color & 0xFF0000) >> 16) | (color & 0xFF00) | ((color & 0xFF) << 16);
        }
        m_sci->setEdgeColour(color);
    }
    if (name == QStringLiteral("White space symbol"))
    {
        QString foreColor = styleElem.attribute(QStringLiteral("fgColor"));
        if (!foreColor.isEmpty())
        {
            int color = foreColor.toLong(nullptr, 16);
            color     = ((color & 0xFF0000) >> 16) | (color & 0xFF00) | ((color & 0xFF) << 16);
            m_sci->setWhitespaceFore(true, color);
        }
    }
}
