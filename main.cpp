#include <QCoreApplication>
#include "mainwindow.h"
#include "settings.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);

    QCoreApplication::setApplicationName("Code Inspector");
    QCoreApplication::setApplicationVersion("1.0");

    QApplication a(argc, argv);

    g_settings = new Settings;
    g_settings->initialize();

    MainWindow w;
    w.show();

    return a.exec();
}
