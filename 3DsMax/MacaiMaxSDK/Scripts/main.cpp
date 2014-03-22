#include "scripts_widget.h"
#include "qwin/qmfcapp.h"
#include "qwin/qwinwidget.h"

#include <windows.h>
#include <QTextCodec>

//#include <QApplication>
//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    ScriptsWidget w;
//    w.show();
    
//    return a.exec();
//}

BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD dwReason, LPVOID /*lpvReserved*/ )
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        QMfcApp::pluginInstance(hinstDLL);
    }

    QTextCodec *codec = QTextCodec::codecForName("gbk");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    return TRUE;
}
