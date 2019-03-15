#include "stdafx.h"
#include "mainwindow.h"
#include "settings.h"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);

    QApplication::setApplicationName("CodeInspector");
    QApplication::setApplicationVersion("1.0");

    QApplication a(argc, argv);

    g_settings = new Settings;
    g_settings->initialize();

    MainWindow w;
    w.show();

    return a.exec();
}
