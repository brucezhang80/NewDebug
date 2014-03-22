#ifndef PICTUREVIEWER_H
#define PICTUREVIEWER_H

#include "win32/Win32Window.h"

class MainWidget;

class PictureViewer
{
public:
    PictureViewer(HWND hParent = 0);
    ~PictureViewer();

    void show();

private:
    Win32Window       *m_win32window;
    MainWidget        *m_mainWidget;
}

#endif // PICTUREVIEWER_H
