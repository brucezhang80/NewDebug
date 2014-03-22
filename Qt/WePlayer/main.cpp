/**************************************************************************
** Qt Creator license header template
**   Special keywords: Administrator 2013/12/12 2013
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/

#include "WePlayer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WePlayer w;
    w.show();

    return a.exec();
}
