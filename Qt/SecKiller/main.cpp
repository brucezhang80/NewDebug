/**************************************************************************
** Qt Creator license header template
**   Special keywords: Administrator 2013/12/6 2013
**   Environment variables:
**   To protect a percent sign, use '%'.
**************************************************************************/

#include "MainWindow.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    MainWindow w;
    w.show();

    return a.exec();
}
