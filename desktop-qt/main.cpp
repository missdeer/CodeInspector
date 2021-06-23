#include <QApplication>
#include <QMessageBox>
#include <QPixmap>
#include <QPluginLoader>
#include <QSplashScreen>

#include "codeinspectorapp.h"
#include "mainwindow.h"
#include "settings.h"

// option plugins
Q_IMPORT_PLUGIN(AstPlugin)
Q_IMPORT_PLUGIN(CfgPlugin)
Q_IMPORT_PLUGIN(GCCTreeRTLPlugin)
Q_IMPORT_PLUGIN(LLVMIRPlugin)
Q_IMPORT_PLUGIN(OptimizationPlugin)

// runner plugins
Q_IMPORT_PLUGIN(RextesterPlugin)
Q_IMPORT_PLUGIN(WandboxPlugin)

// tool plugins
Q_IMPORT_PLUGIN(ClangFormatPlugin)
Q_IMPORT_PLUGIN(ClangQueryPlugin)
Q_IMPORT_PLUGIN(ClangTidyPlugin)
Q_IMPORT_PLUGIN(IncludeWhatYouUsePlugin)
Q_IMPORT_PLUGIN(LDDPlugin)
Q_IMPORT_PLUGIN(LLVMMCAPlugin)
Q_IMPORT_PLUGIN(OSACAPlugin)
Q_IMPORT_PLUGIN(PaholePlugin)
Q_IMPORT_PLUGIN(PSVStudioPlugin)
Q_IMPORT_PLUGIN(ReadELFPlugin)
Q_IMPORT_PLUGIN(StringsPlugin)
Q_IMPORT_PLUGIN(X86To6502Plugin)

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(CodeInspector);
    Q_INIT_RESOURCE(coreImage);
    Q_INIT_RESOURCE(example);
    Q_INIT_RESOURCE(scintilla);
    Q_INIT_RESOURCE(image);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
#endif

    QApplication::setApplicationName("CodeInspector");
    QApplication::setApplicationVersion("1.0");

    QApplication a(argc, argv);

    QPixmap       pixmap(":/CodeInspector.png");
    QSplashScreen splash(pixmap);
    splash.show();
    QApplication::processEvents();
    Settings settings;
    g_settings = &settings;

    splash.showMessage(QObject::tr("Reading settings..."));
    g_settings->initialize();

    if (g_settings->sslRequired() && !QSslSocket::supportsSsl())
    {
        QMessageBox::critical(nullptr, QObject::tr("Critical error"), QObject::tr("SSL not supported, exit now."), QMessageBox::Ok);
        return 1;
    }

    splash.showMessage(QObject::tr("Loading plugins..."));
    const auto staticInstances = QPluginLoader::staticInstances();
    qDebug() << "there are " << staticInstances.length() << " static plugins";

    splash.showMessage(QObject::tr("Creating application instance..."));
    CodeInspectorApp app;
    ciApp = &app;

    splash.showMessage(QObject::tr("Initializing application instance..."));
    ciApp->initialize();

    splash.showMessage(QObject::tr("Creating main window..."));
    MainWindow w;
    w.showMaximized();

    splash.finish(&w);
    return QApplication::exec();
}
