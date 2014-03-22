#ifndef AREOWIDGET_H
#define AREOWIDGET_H

#include <QColor>
#include <QWidget>
#include <QLibrary>
#include <QApplication>
#include <QWidget>
#include <QList>
#include <QPointer>

class AreoWidget;

class QtWin
{
public:
    QtWin();
public:
    static bool enableAeroWindow(QWidget *widget, bool enable = true);
    static bool extendFrameIntoClientArea(QWidget *widget,
                                          int left = -1, int top = -1,
                                          int right = -1, int bottom = -1);
    static bool isCompositionEnabled();
    static QColor colorizatinColor();
private:
    static AreoWidget *windowNotifier();
};

#ifdef Q_WS_WIN
#include <qt_windows.h>
// Blur behind data structures
#define DWM_BB_ENABLE                 0x00000001  // fEnable has been specified
#define DWM_BB_BLURREGION             0x00000002  // hRgnBlur has been specified
#define DWM_BB_TRANSITIONONMAXIMIZED  0x00000004  // fTransitionOnMaximized has been specified
#define WM_DWMCOMPOSITIONCHANGED        0x031E    // Composition changed window message
typedef struct _DWM_BLURBEHIND
{
    DWORD dwFlags;
    BOOL fEnable;
    HRGN hRgnBlur;
    BOOL fTransitionOnMaximized;
} DWM_BLURBEHIND, *PDWM_BLURBEHIND;
typedef struct _MARGINS
{
    int cxLeftWidth;
    int cxRightWidth;
    int cyTopHeight;
    int cyBottomHeight;
} MARGINS, *PMARGINS;

typedef HRESULT (WINAPI *PtrDwmIsCompositionEnabled)(BOOL* pfEnabled);
typedef HRESULT (WINAPI *PtrDwmExtendFrameIntoClientArea)(HWND hWnd, const MARGINS* pMarInset);
typedef HRESULT (WINAPI *PtrDwmenableAeroWindow)(HWND hWnd, const DWM_BLURBEHIND* pBlurBehind);
typedef HRESULT (WINAPI *PtrDwmGetColorizationColor)(DWORD *pcrColorization, BOOL *pfOpaqueBlend);
static PtrDwmIsCompositionEnabled pDwmIsCompositionEnabled= 0;
static PtrDwmenableAeroWindow pDwmenableAeroWindow = 0;
static PtrDwmExtendFrameIntoClientArea pDwmExtendFrameIntoClientArea = 0;
static PtrDwmGetColorizationColor pDwmGetColorizationColor = 0;
/*
 *同步响应DWM状态消息
 */
class AreoWidget : public QWidget
{
public:
    AreoWidget(QWidget *parent = 0);
    void addWidget(QWidget *widget) { widgets.append(widget); }
    void removeWidget(QWidget *widget) { widgets.removeAll(widget); }
    bool winEvent(MSG *message, long *result);
private:
    QWidgetList widgets;
};

static bool resolveLibs()
{
    if (!pDwmIsCompositionEnabled)
    {
        QLibrary dwmLib(QString::fromAscii("dwmapi"));
        pDwmIsCompositionEnabled =(PtrDwmIsCompositionEnabled)dwmLib.resolve("DwmIsCompositionEnabled");
        pDwmExtendFrameIntoClientArea = (PtrDwmExtendFrameIntoClientArea)dwmLib.resolve("DwmExtendFrameIntoClientArea");
        pDwmenableAeroWindow = (PtrDwmenableAeroWindow)dwmLib.resolve("DwmenableAeroWindow");
        pDwmGetColorizationColor = (PtrDwmGetColorizationColor)dwmLib.resolve("DwmGetColorizationColor");
    }
    return pDwmIsCompositionEnabled != 0;
}
#endif

#endif // AREOWIDGET_H
