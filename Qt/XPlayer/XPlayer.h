/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:XPlayer.h
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

#ifndef XPLAYER_H
#define XPLAYER_H

#include "MainWindowEx/MainWindowEx.h"
#include "MediaPlayWidget/MediaPlayWidget.h"
#include "MediaHintButtonWidget/MediaHintButtonWidget.h"
#include "MediaAlbumWidget/MediaAlbumWidget.h"
#include "MediaPlayListWidget/MediaPlayListWidget.h"
#include "MediaSearchWidget/MediaSearchWidget.h"
#include "MediaProgressBar/MediaProgressBar.h"
#include "StateMachineServer/StateMachineServer.h"
#include "MediaDataManager/NetMusicManager.h"

#include <QLabel>


class XPlayer : public QObject
{
public:
    XPlayer(QObject * parent = 0);
    ~XPlayer();

    void setup();
    void enableAnimate(bool enabled = true);

private:
    void setHintButtonSignals();
    void setMediaPlaySignals();
    void setMediaItemSignals();
    void setSearchSignals();

private:
    MainWindowEx*           m_mainWindowEx;
    MediaSearchWidget*      m_mediaSearchWidget;
    MediaHintButtonWidget*  m_mediaHintButtonWidget;
    MediaAlbumWidget*       m_mediaAlbumWidget;
    MediaPlayListWidget*    m_mediaPlayListWidget;
    MediaPlayWidget*        m_mediaPlayWidget;
    MediaProgressBar*       m_mediaProgressBar;
//    StateMachineServer*     m_stateMachine;

    NetMusicManager*        m_netMusicManager;

    QLabel*                 m_windowTitle;

};

#endif // XPLAYER_H
