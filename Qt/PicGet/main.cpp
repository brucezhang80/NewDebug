#include "uniqueapp.h"
#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);
    UniqueApp ua(argc, argv);
    QTextCodec* codec = QTextCodec::codecForName("system");
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForTr(codec);
    if( ua.isRunning() )
    {
        QMessageBox::information( 0, "警告", "只能运行一个实例" );
        return 0;
    }

    MainWindow w;
    w.resize(840, 480);
    w.show();

    return ua.exec();
}
