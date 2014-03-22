/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:XPlayer.cpp
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


#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QSplitter>
#include <QGridLayout>
#include <QFile>

#include <QDebug>

XPlayer::XPlayer(QObject *parent)
    : QObject(parent)
{
    m_mainWindowEx = new MainWindowEx;
    m_mainWindowEx->mainMenuBar()->hide();
    m_mainWindowEx->mainToolBar()->hide();
    m_mainWindowEx->mainStatusBar()->hide();
    QWidget *centralWidget = m_mainWindowEx->centralWidget();

    QGridLayout *centralGrid = new QGridLayout(centralWidget);
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setSpacing(6);

    m_mediaSearchWidget = new MediaSearchWidget(centralWidget);
    gridLayout->addWidget(m_mediaSearchWidget, 0, 0, 1, 1);

    QSpacerItem* hLeftSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(hLeftSpacer, 0, 1, 1, 1);
    m_windowTitle = new QLabel( centralWidget);
    m_windowTitle->setText( tr("XPlay - Untitle") );
    m_windowTitle->setAlignment(Qt::AlignCenter);
    m_windowTitle->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    gridLayout->addWidget(m_windowTitle, 0, 2, 1, 1);
    QSpacerItem* hRightSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(hRightSpacer, 0, 3, 1, 1);

    m_mediaHintButtonWidget = new MediaHintButtonWidget(centralWidget);
    gridLayout->addWidget(m_mediaHintButtonWidget, 0, 4, 1, 1);

    // center widget with a splitter
    QSplitter* splitter = new QSplitter(centralWidget);
    splitter->setOrientation(Qt::Horizontal);
    m_mediaAlbumWidget = new MediaAlbumWidget(splitter);
    splitter->addWidget(m_mediaAlbumWidget);
    m_mediaPlayListWidget = new MediaPlayListWidget(splitter);
    splitter->addWidget(m_mediaPlayListWidget);
    splitter->setStretchFactor(0, 25);
    splitter->setStretchFactor(1, 75);
//    splitter->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    gridLayout->addWidget(splitter, 1, 0, 1, 5);

    m_mediaProgressBar = new MediaProgressBar(centralWidget);
    gridLayout->addWidget(m_mediaProgressBar, 2, 0, 1, 5);
    m_mediaProgressBar->setObjectName("MediaProgressBar");

    QSpacerItem* leftSpacer = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem* rightSpacer = new QSpacerItem(120, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    gridLayout->addItem(leftSpacer, 3, 0, 1, 2);
    m_mediaPlayWidget = new MediaPlayWidget(centralWidget);
    gridLayout->addWidget(m_mediaPlayWidget, 3, 2, 1, 1);
    gridLayout->addItem(rightSpacer, 3, 3, 1, 2);
    centralGrid->addLayout(gridLayout, 0, 0, 1, 1);

    m_netMusicManager = new NetMusicManager();
    //m_netMusicManager->moveToThread(netMusicThread);

    setMediaItemSignals();
    setHintButtonSignals();
    setMediaPlaySignals();
    setSearchSignals();
}

XPlayer::~XPlayer()
{
    m_mainWindowEx->deleteLater();
    m_mediaSearchWidget->deleteLater();
    m_mediaHintButtonWidget->deleteLater();
    m_mediaAlbumWidget->deleteLater();
    m_mediaPlayListWidget->deleteLater();
    m_mediaPlayWidget->deleteLater();
    m_mediaProgressBar->deleteLater();
    m_windowTitle->deleteLater();
}

void XPlayer::setup()
{
    QPixmap pixmap = QPixmap(":/images/Cover.jpg");
    m_mainWindowEx->setBackgroundCover(pixmap);
    m_mainWindowEx->show();
}

void XPlayer::enableAnimate(bool enabled)
{
    if( enabled )
    {
//        connect(m_mainWindowEx, SIGNAL(sizeChanged()), m_mediaAlbumWidget, SLOT(resetAnimate()) );
//        connect(m_mainWindowEx, SIGNAL(sizeChanged()), m_mediaHintButtonWidget, SLOT(resetAnimate()) );

        connect(m_mainWindowEx, SIGNAL(hoverEnter()),m_mediaSearchWidget, SIGNAL(animateShow()) );
        connect(m_mainWindowEx, SIGNAL(hoverLeave()),m_mediaSearchWidget, SIGNAL(animateHide()) );

        connect(m_mainWindowEx, SIGNAL(hoverEnter()),m_mediaHintButtonWidget, SIGNAL(animateShow()) );
        connect(m_mainWindowEx, SIGNAL(hoverLeave()),m_mediaHintButtonWidget, SIGNAL(animateHide()) );

        connect(m_mainWindowEx, SIGNAL(hoverEnter()),m_mediaAlbumWidget, SIGNAL(animateShow()) );
        connect(m_mainWindowEx, SIGNAL(hoverLeave()),m_mediaAlbumWidget, SIGNAL(animateHide()) );

        connect(m_mainWindowEx, SIGNAL(hoverEnter()),m_mediaPlayListWidget, SIGNAL(animateShow()) );
        connect(m_mainWindowEx, SIGNAL(hoverLeave()),m_mediaPlayListWidget, SIGNAL(animateHide()) );

        connect(m_mainWindowEx, SIGNAL(hoverEnter()),m_mediaProgressBar, SIGNAL(animateShow()) );
        connect(m_mainWindowEx, SIGNAL(hoverLeave()),m_mediaProgressBar, SIGNAL(animateHide()) );

        connect(m_mainWindowEx, SIGNAL(hoverEnter()),m_mediaPlayWidget, SIGNAL(animateShow()) );
        connect(m_mainWindowEx, SIGNAL(hoverLeave()),m_mediaPlayWidget, SIGNAL(animateHide()) );
    }
    else
    {
        disconnect(m_mainWindowEx, SIGNAL(hoverEnter()),m_mediaSearchWidget, SIGNAL(animateShow()) );
        disconnect(m_mainWindowEx, SIGNAL(hoverLeave()),m_mediaSearchWidget, SIGNAL(animateHide()) );

        disconnect(m_mainWindowEx, SIGNAL(hoverEnter()),m_mediaHintButtonWidget, SIGNAL(animateShow()) );
        disconnect(m_mainWindowEx, SIGNAL(hoverLeave()),m_mediaHintButtonWidget, SIGNAL(animateHide()) );

        disconnect(m_mainWindowEx, SIGNAL(hoverEnter()),m_mediaAlbumWidget, SIGNAL(animateShow()) );
        disconnect(m_mainWindowEx, SIGNAL(hoverLeave()),m_mediaAlbumWidget, SIGNAL(animateHide()) );

        disconnect(m_mainWindowEx, SIGNAL(hoverEnter()),m_mediaPlayListWidget, SIGNAL(animateShow()) );
        disconnect(m_mainWindowEx, SIGNAL(hoverLeave()),m_mediaPlayListWidget, SIGNAL(animateHide()) );

        disconnect(m_mainWindowEx, SIGNAL(hoverEnter()),m_mediaProgressBar, SIGNAL(animateShow()) );
        disconnect(m_mainWindowEx, SIGNAL(hoverLeave()),m_mediaProgressBar, SIGNAL(animateHide()) );

        disconnect(m_mainWindowEx, SIGNAL(hoverEnter()),m_mediaPlayWidget, SIGNAL(animateShow()) );
        disconnect(m_mainWindowEx, SIGNAL(hoverLeave()),m_mediaPlayWidget, SIGNAL(animateHide()) );
    }
}

void XPlayer::setMediaItemSignals()
{
//    connect( m_mediaPlayListWidget, SIGNAL(autoHide()),
//             m_stateMachine, SIGNAL(transHide()) );
}

void XPlayer::setSearchSignals()
{
    connect( m_mediaSearchWidget, SIGNAL(searchMusic(QString)),
             m_netMusicManager, SLOT(searchMusic(QString)) );
}

void XPlayer::setHintButtonSignals()
{
    connect( m_mediaHintButtonWidget, SIGNAL(closed()), m_mainWindowEx, SLOT(close()) );
    connect( m_mediaHintButtonWidget, SIGNAL(maximized()), m_mainWindowEx, SLOT(showMaximize()) );
    connect( m_mediaHintButtonWidget, SIGNAL(minimized()), m_mainWindowEx, SLOT(showMinimized()) );
}

void XPlayer::setMediaPlaySignals()
{

}
