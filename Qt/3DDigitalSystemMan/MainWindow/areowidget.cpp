#include "areowidget.h"

AreoWidget::AreoWidget(QWidget *parent):
    QWidget(parent)
{
    winId();
}

/*!
  * 对一个widget实现Aero效果.
  *
  */
bool QtWin::enableAeroWindow(QWidget *widget, bool enable)
{
    Q_ASSERT(widget);
    bool result = false;
#ifdef Q_WS_WIN
    if (resolveLibs())
    {
        DWM_BLURBEHIND bb = {0};
        HRESULT hr = S_OK;
        bb.fEnable = enable;
        bb.dwFlags = DWM_BB_ENABLE;
        bb.hRgnBlur = NULL;
        widget->setAttribute(Qt::WA_TranslucentBackground, enable);
        widget->setAttribute(Qt::WA_NoSystemBackground, enable);
        hr = pDwmenableAeroWindow(widget->winId(), &bb);
        if (SUCCEEDED(hr))
        {
            result = true;
            windowNotifier()->addWidget(widget);
        }
    }
#endif
    return result;
}

/*!
  * 设置Aero绘图区
  */
bool QtWin::extendFrameIntoClientArea(QWidget *widget, int left, int top, int right, int bottom)
{
    Q_ASSERT(widget);
    Q_UNUSED(left);
    Q_UNUSED(top);
    Q_UNUSED(right);
    Q_UNUSED(bottom);
    bool result = false;
#ifdef Q_WS_WIN
    if (resolveLibs())
    {
        QLibrary dwmLib(QString::fromAscii("dwmapi"));
        HRESULT hr = S_OK;
        MARGINS m = {left, top, right, bottom};
        hr = pDwmExtendFrameIntoClientArea(widget->winId(), &m);
        if (SUCCEEDED(hr))
        {
            result = true;
            windowNotifier()->addWidget(widget);
        }
        widget->setAttribute(Qt::WA_TranslucentBackground, result);
    }
#endif
    return result;
}

/*!
  * 检查 DWM 是否开启
  *
  */
bool QtWin::isCompositionEnabled()
{
#ifdef Q_WS_WIN
    if (resolveLibs())
    {
        HRESULT hr = S_OK;
        BOOL isEnabled = false;
        hr = pDwmIsCompositionEnabled(&isEnabled);
        if (SUCCEEDED(hr))
            return isEnabled;
    }
#endif
    return false;
}

// 返回当前窗口颜色.
QColor QtWin::colorizatinColor()
{
    QColor resultColor = QApplication::palette().window().color();
#ifdef Q_WS_WIN
    if (resolveLibs())
    {
        DWORD color = 0;
        BOOL opaque = FALSE;
        QLibrary dwmLib(QString::fromAscii("dwmapi"));
        HRESULT hr = S_OK;
        hr = pDwmGetColorizationColor(&color, &opaque);
        if (SUCCEEDED(hr))
            resultColor = QColor(color);
    }
#endif
    return resultColor;
}

#ifdef Q_WS_WIN
AreoWidget *QtWin::windowNotifier()
{
    static AreoWidget *windowNotifierInstance = 0;
    if (!windowNotifierInstance)
        windowNotifierInstance = new AreoWidget;
    return windowNotifierInstance;
}

/* 所有窗口响应 DWM 状态变换消息 */
bool AreoWidget::winEvent(MSG *message, long *result)
{
    if (message && message->message == WM_DWMCOMPOSITIONCHANGED) {
        bool compositionEnabled = QtWin::isCompositionEnabled();
        foreach(QWidget * widget, widgets) {
            if (widget) {
                widget->setAttribute(Qt::WA_NoSystemBackground, compositionEnabled);
            }
            widget->update();
        }
    }
    return QWidget::winEvent(message, result);
}
#endif

