#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QTextCodec>
#include <QFile>
#include <QTranslator>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec* codec = QTextCodec::codecForName("SYSTEM");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    QTranslator translator;
    if( !translator.load(":/lang/qt_zh_cn"))
    {
        qDebug() << "load lang file failed .";
    }
    else
        a.installTranslator(&translator);

    MainWindow w;

//    QFile qss(":/css/css.qss");
//    qss.open(QFile::ReadOnly);
//    w.setStyleSheet(qss.readAll());
//    qss.close();

    w.resize(860,540);
    w.show();
    
    return a.exec();
}
