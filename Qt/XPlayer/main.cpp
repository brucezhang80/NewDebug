/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:main.cpp
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

#include "XPlayer.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);

    QFile file(":/style.qss");
    if( file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        QByteArray data = file.readAll();
        a.setStyleSheet(data);
    }

    XPlayer w;
    w.setup();
    w.enableAnimate(false);

    return a.exec();
}
