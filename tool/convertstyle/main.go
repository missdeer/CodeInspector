package main

import (
	"encoding/xml"
	"flag"
	"fmt"
	"os"
	"path"
	"path/filepath"
	"strings"

	"golang.org/x/net/html/charset"
)

type WordsStyle struct {
	XMLName         xml.Name `xml:"WordsStyle"`
	Name            string   `xml:"name,attr"`
	StyleID         int      `xml:"styleID,attr"`
	ForegroundColor string   `xml:"fgColor,attr"`
	BackgroundColor string   `xml:"bgColor,attr"`
	FontName        string   `xml:"fontName,attr"`
	FontStyle       int      `xml:"fontStyle,attr"`
	FontSize        int      `xml:"fontSize,attr"`
	KeywordClass    string   `xml:"keywordClass,attr"`
}

type LexerType struct {
	XMLName     xml.Name     `xml:"LexerType"`
	WordsStyles []WordsStyle `xml:"WordsStyle"`
	Name        string       `xml:"name,attr"`
	Desc        string       `xml:"desc,attr"`
	Ext         string       `xml:"ext,attr"`
}

type LexerStyles struct {
	XMLName    xml.Name    `xml:"LexerStyles"`
	LexerTypes []LexerType `xml:"LexerType"`
}

type WidgetStyle struct {
	XMLName         xml.Name `xml:"WidgetStyle"`
	Name            string   `xml:"name,attr"`
	StyleID         int      `xml:"styleID,attr"`
	ForegroundColor string   `xml:"fgColor,attr"`
	BackgroundColor string   `xml:"bgColor,attr"`
	FontName        string   `xml:"fontName,attr"`
	FontStyle       int      `xml:"fontStyle,attr"`
	FontSize        int      `xml:"fontSize,attr"`
	KeywordClass    string   `xml:"keywordClass,attr"`
}

type GlobalStyles struct {
	XMLName      xml.Name      `xml:"GlobalStyles"`
	WidgetStyles []WidgetStyle `xml:"WidgetStyle"`
}

type StyleDefinition struct {
	XMLName      xml.Name     `xml:"NotepadPlus"`
	LexerStyles  LexerStyles  `xml:"LexerStyles"`
	GlobalStyles GlobalStyles `xml:"GlobalStyles"`
}

func main() {
	flag.Parse()
	args := flag.Args()
	for _, arg := range args {
		fmt.Println(arg)
		fd, err := os.OpenFile(arg, os.O_RDONLY, 0644)
		if err != nil {
			fmt.Println(err)
			continue
		}
		defer fd.Close()

		var sd StyleDefinition

		decoder := xml.NewDecoder(fd)
		decoder.CharsetReader = charset.NewReaderLabel
		err = decoder.Decode(&sd)
		if err != nil {
			fmt.Println(err)
			continue
		}

		basename := filepath.Base(arg)
		basename = basename[:len(basename)-len(filepath.Ext(basename))]
		fmt.Println(basename)

		themeDirName := path.Join(filepath.Dir(arg), "theme")
		os.MkdirAll(themeDirName, 0755)
		fn := filepath.Join(themeDirName, basename+".lua")
		fdTheme, err := os.OpenFile(fn, os.O_CREATE|os.O_TRUNC|os.O_WRONLY, 0644)
		if err != nil {
			fmt.Println(err)
			continue
		}
		defer fdTheme.Close()
		fdTheme.WriteString("styles = {}\n")
		for _, style := range sd.GlobalStyles.WidgetStyles {
			fdTheme.WriteString(fmt.Sprintf(`styles.%s = { id=%d, name="%s"`,
				strings.ToLower(strings.Replace(style.Name, " ", "_", -1)), style.StyleID, style.Name))
			if style.KeywordClass != "" {
				fdTheme.WriteString(fmt.Sprintf(`, keywordClass="%s"`, style.KeywordClass))
			}
			if style.BackgroundColor != "" {
				fdTheme.WriteString(fmt.Sprintf(`, bgColor="%s"`, style.BackgroundColor))
			}
			if style.ForegroundColor != "" {
				fdTheme.WriteString(fmt.Sprintf(`, fgColor="%s"`, style.ForegroundColor))
			}
			if style.FontName != "" {
				fdTheme.WriteString(fmt.Sprintf(`, fontName="%s"`, style.FontName))
			}
			if style.FontSize != 0 {
				fdTheme.WriteString(fmt.Sprintf(`, fontSize=%d`, style.FontSize))

			}
			fdTheme.WriteString(fmt.Sprintf(", fontStyle=%d}\n", style.FontStyle))
		}
		fdTheme.WriteString("\ntheme = {}\n")

		langDirName := path.Join(filepath.Dir(arg), "lang")
		os.MkdirAll(langDirName, 0755)
		styleExist := map[string]struct{}{}
		count := 0
		for _, ls := range sd.LexerStyles.LexerTypes {
			count += len(ls.WordsStyles)
			for _, style := range ls.WordsStyles {
				name := strings.ToLower(strings.Replace(style.Name, " ", "_", -1))
				val, ok := styleExist[name]
				fmt.Println(name, val, ok, len(styleExist))
				if !ok {
					styleExist[name] = struct{}{}
					fdTheme.WriteString(fmt.Sprintf(`theme.%s = {  name="%s"`, name, style.Name))
					if style.KeywordClass != "" {
						fdTheme.WriteString(fmt.Sprintf(`, keywordClass="%s"`, style.KeywordClass))
					}
					if style.BackgroundColor != "" {
						fdTheme.WriteString(fmt.Sprintf(`, bgColor="%s"`, style.BackgroundColor))
					}
					if style.ForegroundColor != "" {
						fdTheme.WriteString(fmt.Sprintf(`, fgColor="%s"`, style.ForegroundColor))
					}
					if style.FontName != "" {
						fdTheme.WriteString(fmt.Sprintf(`, fontName="%s"`, style.FontName))
					}
					if style.FontSize != 0 {
						fdTheme.WriteString(fmt.Sprintf(`, fontSize=%d`, style.FontSize))

					}
					fdTheme.WriteString(fmt.Sprintf(", fontStyle=%d}\n", style.FontStyle))
				}
			}
		}
		fmt.Println("totally styles count:", count)
		for _, ls := range sd.LexerStyles.LexerTypes {
			fn := filepath.Join(langDirName, ls.Name+".lua")
			if _, err = os.Stat(fn); os.IsNotExist(err) {
				fdLang, err := os.OpenFile(fn, os.O_CREATE|os.O_TRUNC|os.O_WRONLY, 0644)
				if err != nil {
					fmt.Println(err)
					continue
				}
				defer fdLang.Close()
				for _, style := range ls.WordsStyles {
					name := strings.ToLower(strings.Replace(style.Name, " ", "_", -1))

					fdLang.WriteString(fmt.Sprintf(`styles.%s = { id=%d, name="%s"`,
						name, style.StyleID, style.Name))
					if style.KeywordClass != "" {
						fdLang.WriteString(fmt.Sprintf(`, keywordClass="%s"`, style.KeywordClass))
					}
					if style.BackgroundColor != "" {
						fdLang.WriteString(fmt.Sprintf(`, bgColor=theme.%s.bgColor`, name))
					}
					if style.ForegroundColor != "" {
						fdLang.WriteString(fmt.Sprintf(`, fgColor=theme.%s.fgColor`, name))
					}
					if style.FontName != "" {
						fdLang.WriteString(fmt.Sprintf(`, fontName=theme.%s.fontName`, name))
					}
					if style.FontSize != 0 {
						fdLang.WriteString(fmt.Sprintf(`, fontSize=theme.%s.fontSize`, name))

					}
					fdLang.WriteString(fmt.Sprintf(", fontStyle=theme.%s.fontStyle}\n", name))
				}
			}
		}
	}

}
