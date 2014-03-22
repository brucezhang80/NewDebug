/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:MainWindowEx.cpp
**   Author: Young Yori
**   Data: Administrator 2014/2/8 2014
**   Description:
**          This file is part of the Data Development Department Project and shall
**      only be used, modified, and distributed under the terms of the Data
**      Development Department this Source.
**
**      Included in the build system of the FreeType library.
**--------------------------------------------------------------------------------
**                      www.newdebug.com, 12319597@qq.com
**************************************************************************/

#include "MainWindowEx.h"
#include "ui_MainWindowEx.h"


#include <QApplication>
#include <QDesktopWidget>
#include <QStyle>
#include <QPainter>
#include <QMouseEvent>
#include <QHoverEvent>
#include <QTimer>

#include <QDebug>

#include "Windows.h"
#include "WinUser.h"
#include "windowsx.h"
#include "dwmapi.h"

#include <QDebug>

MainWindowEx::MainWindowEx(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowEx)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_TranslucentBackground, false);
    this->setAttribute(Qt::WA_NoSystemBackground, false);
    this->resize(680, 420);

    // Handle window creation.
    RECT rcClient;
    GetWindowRect((HWND)this->winId(), &rcClient);

//    qDebug() << rcClient.bottom << rcClient.left << rcClient.right << rcClient.top;
//     Inform application of the frame change.
    SetWindowPos((HWND)this->winId(),
                 NULL,
                 rcClient.left, rcClient.top,
                 rcClient.right - rcClient.left,
                 rcClient.bottom - rcClient.top,
                 SWP_FRAMECHANGED);
}

MainWindowEx::~MainWindowEx()
{
    delete ui;
}

QMenuBar *MainWindowEx::mainMenuBar() const
{
    return ui->menuBar;
}

QToolBar *MainWindowEx::mainToolBar() const
{
    return ui->mainToolBar;
}

QStatusBar *MainWindowEx::mainStatusBar() const
{
    return ui->statusBar;
}

void MainWindowEx::setBackgroundCover(/*const*/ QPixmap &pixmap)
{
    m_backgroundCover = pixmap;
    QPixmap temp = m_backgroundCover.copy(m_backgroundCover.rect());
    QPalette palette(this->palette());
    palette.setBrush( QPalette::Window, QBrush(temp.scaled(this->size())) );
    this->setPalette(palette);
}

void MainWindowEx::showMaximize()
{
    //使用Qt的showNormal()来还原窗口, 导致窗口上的部件失去鼠标事件(单击)
    //使用系统API还原窗口得以解决
    //(Qt5中没有这种问题出现)
    this->isMaximized() ? ShowWindow( (HWND)this->winId(),SW_NORMAL )/*this->showNormal()*/:\
                          this->showMaximized();
}

bool MainWindowEx::hitTestNCA(MSG *msg, long *result)
{
    // Get the window rectangle.
    RECT rcWindow;
    GetWindowRect(msg->hwnd, &rcWindow);

    // Get the frame rectangle, adjusted for the style without a caption.
    RECT rcFrame = { 0 };
    AdjustWindowRectEx(&rcFrame, WS_OVERLAPPEDWINDOW & ~WS_CAPTION, FALSE, NULL);

    int x = GET_X_LPARAM(msg->lParam);
    int y = GET_Y_LPARAM(msg->lParam);
    //    qDebug() << this->childAt(QPoint(x - rcWindow.left, y - rcWindow.top))->objectName();
    if (this->childAt(QPoint(x - rcWindow.left, y - rcWindow.top)) == ui->centralWidget)
    {
        USHORT uRow = 1;
        USHORT uCol = 1;

        // Determine if the point is at the top or bottom of the window.
        if (y >= rcWindow.top && y < rcWindow.top + style()->pixelMetric(QStyle::PM_MDIFrameWidth))
            uRow = 0;
        else if (y < rcWindow.bottom && y >= rcWindow.bottom - style()->pixelMetric(QStyle::PM_MDIFrameWidth))
            uRow = 2;

        // Determine if the point is at the left or right of the window.
        if (x >= rcWindow.left && x < rcWindow.left + style()->pixelMetric(QStyle::PM_TitleBarHeight))
            uCol = 0;
        else if (x < rcWindow.right && x >= rcWindow.right - style()->pixelMetric(QStyle::PM_MDIFrameWidth))
            uCol = 2;

        LRESULT hitTests[3][3] =
                {
                    { HTTOPLEFT,    HTTOP,          HTTOPRIGHT },
                    { HTLEFT,       HTCAPTION,      HTRIGHT },
                    { HTBOTTOMLEFT, HTBOTTOM,       HTBOTTOMRIGHT },
                };

        *result = hitTests[uRow][uCol];
        return true;
    }
    else
    {
        *result = HTCLIENT;
        return true;
    }

    *result = HTNOWHERE;

    return false;
}

bool MainWindowEx::winEvent(MSG *msg, long *result)
{
    bool fCallDWP = true;
    BOOL fDwmEnabled = FALSE;
    LRESULT lRet = 0;
    HRESULT hr = S_OK;
    HWND hWnd       = msg->hwnd;
    UINT message    = msg->message;
    WPARAM wParam   = msg->wParam;
    LPARAM lParam   = msg->lParam;

    // Winproc worker for custom frame issues.
    hr = DwmIsCompositionEnabled(&fDwmEnabled);
    if (!SUCCEEDED(hr) || !fDwmEnabled)
    {
        qDebug() << "DWM not enabled";
        return false;
    }
    fCallDWP = !DwmDefWindowProc(hWnd, message, wParam, lParam, &lRet);

    switch(message)
    {
        // 鼠标在窗口边缘时，缩放可用
        case WM_NCHITTEST:
            return hitTestNCA(msg, result);

        // Handle window activation.
        case WM_ACTIVATE:
        {
            // Extend the frame into the client area.
            MARGINS margins = { -1, -1, -1, -1 };
            hr = DwmExtendFrameIntoClientArea(hWnd, &margins);

            if (!SUCCEEDED(hr))
            {
                // Handle error.
                qDebug() << "Extend frame to Client Area error";
            }

            fCallDWP = true;
            lRet = 0;
        };
        break;

        // 缩放
        case WM_NCCALCSIZE:
        {
            if(wParam == TRUE)
            {
                // Calculate new NCCALCSIZE_PARAMS based on custom NCA inset.
                NCCALCSIZE_PARAMS *pncsp = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);

                pncsp->rgrc[0].left   = pncsp->rgrc[0].left   - 1;
                pncsp->rgrc[0].top    = pncsp->rgrc[0].top    - 1;
                pncsp->rgrc[0].right  = pncsp->rgrc[0].right  + 1;
                pncsp->rgrc[0].bottom = pncsp->rgrc[0].bottom + 1;

                QRect rect;
                rect.setLeft(pncsp->rgrc[0].left);
                rect.setTop(pncsp->rgrc[0].top);
                rect.setRight(pncsp->rgrc[0].right);
                rect.setBottom(pncsp->rgrc[0].bottom);
                emit sizeChanged();

                lRet = 0;
                // No need to pass the message on to the DefWindowProc.
                fCallDWP = false;
            }
        };
        break;

        case WM_NCMOUSELEAVE:
        {
//    if( uRow ==0 || uRow == 2 || uCol == 0 || uCol == 2)
//        emit hoverLeave();
//    else
//        emit hoverEnter();
            if(!this->geometry().contains(QCursor::pos()))
            {
//                QTimer::singleShot(400, this, SIGNAL(hoverLeave()));
                emit hoverLeave();
            }
        };
        break;

        case WM_NCMOUSEMOVE:
        {
            emit hoverEnter();
        };
        break;
        //最大化/最小化
        case WM_GETMINMAXINFO:
        {
            MINMAXINFO *mmi = (MINMAXINFO*)lParam;
            QRect rect = QApplication::desktop()->availableGeometry();
//            mmi->ptMaxSize.x = rect.width();
//            mmi->ptMaxSize.y = rect.height();
            mmi->ptMaxPosition.x = 0;
            mmi->ptMaxPosition.y = 0;
            mmi->ptMinTrackSize.x = 400;
            mmi->ptMinTrackSize.y = 260;
            mmi->ptMaxTrackSize.x = rect.width();
            mmi->ptMaxTrackSize.y = rect.height();

            lRet = 0;
            fCallDWP = false;
        };
        break;
    }

    if (!fCallDWP)
        *result = lRet;

    return !fCallDWP;
}

bool MainWindowEx::nativeEvent(const QByteArray &/*eventType*/, void *message, long *result)
{
    return winEvent((MSG*)message, result);
}

void MainWindowEx::resizeEvent(QResizeEvent *event)
{
    if( m_backgroundCover.isNull() ) return;

    QPixmap temp = m_backgroundCover.copy(m_backgroundCover.rect());
    QPalette pal(palette());
    pal.setBrush(QPalette::Window,QBrush(temp.scaled(event->size())));
    setPalette(pal);
}



