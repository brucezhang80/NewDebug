#include "MainWidget.h"

#include <qt_windows.h>
#include <QApplication>

static BOOL enumUserWindowsCB(HWND hwnd,LPARAM lParam);
static HWND findDesktopIconWnd();


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWidget w;

//    HWND desktopHwnd = findDesktopIconWnd();
//    if(desktopHwnd) SetParent(w.winId(), desktopHwnd);

    w.show();

    return a.exec();
}

static BOOL enumUserWindowsCB(HWND hwnd,LPARAM lParam)
{
    long wflags = GetWindowLong(hwnd, GWL_STYLE);
    if(!(wflags & WS_VISIBLE)) return TRUE;

    HWND sndWnd;
    if( !(sndWnd=FindWindowEx(hwnd, NULL, "SHELLDLL_DefView", NULL)) ) return TRUE;
    HWND targetWnd;
    if( !(targetWnd=FindWindowEx(sndWnd, NULL, "SysListView32", "FolderView")) ) return TRUE;

    HWND* resultHwnd = (HWND*)lParam;
    *resultHwnd = targetWnd;
    return FALSE;
}

static HWND findDesktopIconWnd()
{
    HWND resultHwnd = NULL;
    EnumWindows((WNDENUMPROC)enumUserWindowsCB, (LPARAM)&resultHwnd);
    return resultHwnd;
}
