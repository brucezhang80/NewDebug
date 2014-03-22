/**************************************************************************
** Qt Creator license header template
**   Special keywords: Administrator 2013/12/20 2013
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/

#include "Widget.h"
#include <QApplication>
#include <QLibrary>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    w.setAttribute(Qt::WA_TranslucentBackground);
    QLibrary dwmapi_dll("dwmapi.dll");
    if ( dwmapi_dll.load() )
    {
        struct Margins
        {
            int left;
            int right;
            int top;
            int bottom;
        };

        typedef void (*pDWM_EFICA) (HWND, const Margins*);
        typedef bool (*pDWM_ENABLED) ();

        pDWM_ENABLED enabled =
                (pDWM_ENABLED) dwmapi_dll.resolve("DwmIsCompositionEnabled");
        // 如果支持Aero明秀效果(vista以上版本可用)
        if( enabled )
        {
            pDWM_EFICA DwmExtendFrameIntoClientArea =
                                (pDWM_EFICA) dwmapi_dll.resolve("DwmExtendFrameIntoClientArea");
            Margins margin = {-1, -1, -1, -1};
            if (DwmExtendFrameIntoClientArea)
            {
                 DwmExtendFrameIntoClientArea((HWND)w.winId(), &margin);
            }
        }

        dwmapi_dll.unload();
    }
    w.show();

    return a.exec();
}
