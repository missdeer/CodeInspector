#include "mainwindow.h"
#include "settings.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    g_settings = new Settings;
    g_settings->initialize();

    MainWindow w;
    w.show();

    return a.exec();
}
