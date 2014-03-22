#include "GraphViewer.h"

#include "qwin/qmfcapp.h"
#include "qwin/qwinwidget.h"

#include <QApplication>
#include <qt_windows.h>
#include <QTextCodec>
#include <QDir>

//#ifdef Q_WS_WIN
//#define SCRIPTS_EXPORT __declspec(dllexport)
//#else
//#define SCRIPTS_EXPORT
//#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GraphViewer w;
    w.show();

    return a.exec();
}

//GraphViewer *picViewer = 0;

//BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD dwReason, LPVOID /*lpvReserved*/ )
//{
//    if (dwReason == DLL_PROCESS_ATTACH)
//    {
//        QMfcApp::pluginInstance(hinstDLL);
//    }
//    QTextCodec *codec = QTextCodec::codecForName("GBK");
//    QTextCodec::setCodecForLocale(codec);
//    QTextCodec::setCodecForCStrings(codec);
//    QTextCodec::setCodecForTr(codec);

//    return TRUE;
//}

//extern "C" __declspec(dllexport) const bool show(HWND hParent)
//{
//    QWinWidget* winWidget = new QWinWidget( hParent );
//        winWidget->setContentsMargins(0, 0, 0, 0);
//    if( picViewer == 0 )
//        picViewer = new GraphViewer(winWidget);

//    picViewer->show();

//    return true;
//}

//extern "C" __declspec(dllexport) const bool close(HWND /*hParent*/)
//{
//    QMfcApp::quit();
//    return true;
//}
