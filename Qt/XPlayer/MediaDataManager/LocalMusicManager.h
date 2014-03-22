/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:LocalMusicManager.h
**   Author: Young Yori
**   Data: Administrator 2014/2/19 2014
**   Description:
**          This file is part of the Data Development Department Project and shall
**      only be used, modified, and distributed under the terms of the Data
**      Development Department this Source.
**
**      Included in the build system of the FreeType library.
**--------------------------------------------------------------------------------
**                      www.newdebug.com, 12319597@qq.com
**************************************************************************/

#ifndef LOCALMUSICMANAGER_H
#define LOCALMUSICMANAGER_H

#include <QObject>
#include "DataManager.h"

class LocalMusicManager : public DataManager
{
    Q_OBJECT
public:
    explicit LocalMusicManager(QObject *parent = 0);
    ~LocalMusicManager();

signals:

public slots:

};

#endif // LOCALMUSICMANAGER_H
