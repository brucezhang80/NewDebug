#include "PictureViewer.h"
#include "MainWidget.h"
#include "qwin/qmfcapp.h"
#include "qwin/qwinwidget.h"
#include <QGridLayout>

PictureViewer::PictureViewer(HWND hParent)
{
    m_win32window = new Win32Window( hParent );
    m_win32window->create( "Picture Viewer" );
    HWND hwnd = m_win32window->getWnd();
    QWinWidget* winWidget = new QWinWidget( hwnd );
//    winWidget->resize(400, 300);
    winWidget->setContentsMargins(0, 0, 0, 0);

    m_mainWidget = new MainWidget(winWidget);
    QGridLayout *gridlayout = new QGridLayout;
    gridlayout->setContentsMargins(0, 0, 10, 30);
    gridlayout->setSpacing(0);
    gridlayout->addWidget( m_mainWidget );
    winWidget->setLayout( gridlayout );
    m_win32window->setWidget(winWidget);

}

PictureViewer::~PictureViewer()
{
    m_mainWidget->deleteLater();
}

void PictureViewer::show()
{
    m_win32window->show();
}
