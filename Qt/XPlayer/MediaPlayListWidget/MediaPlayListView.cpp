/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:MediaItemView.cpp
**   Author: Young Yori
**   Data: Administrator 2014/2/18 2014
**   Description:
**          This file is part of the Data Development Department Project and shall
**      only be used, modified, and distributed under the terms of the Data
**      Development Department this Source.
**
**      Included in the build system of the FreeType library.
**--------------------------------------------------------------------------------
**                      www.newdebug.com, 12319597@qq.com
**************************************************************************/

#include "MediaPlayListView.h"
#include "AlbumList.h"

#include <QHeaderView>
#include <QStringList>

MediaPlayListView::MediaPlayListView(QWidget *parent) :
    QTreeWidget(parent)
{
// 设置treewidget属性
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setRootIsDecorated(false);
    this->setAcceptDrops(true);
    this->setMouseTracking(true);
    this->setExpandsOnDoubleClick(false);

// 初始化标头
    QStringList headers;
    headers << tr("#") << tr("Title")  <<tr("Length") << tr("Artist") << tr("Rate");
    this->setHeaderLabels(headers);
    QFontMetrics fm = fontMetrics();
    QHeaderView* header = this->header();
//    header->resizeSection(0, qMax(fm.width(headers.at(0)+ "####"), fm.width(" Album Album Album Album ")) );
    header->resizeSection(0, qMax(fm.width(headers.at(0)+"####"), fm.width(" 12345 ")));
    header->resizeSection(1, qMax(fm.width(headers.at(1)+"####"), 2*fm.width(" name-for-a-file.mp3")));
    header->resizeSection(2, qMax(fm.width(headers.at(2)+"####"), fm.width(" hh:mm:ss   ")) );
    header->resizeSection(3, qMax(fm.width(headers.at(3)+"####"), fm.width(" Artist Artist")) );
    header->resizeSection(4, qMax(fm.width(headers.at(4)+"####"), fm.width(" ☆☆☆☆☆ ")) );

//    this->setStyleSheet("background-color: rgba( 255, 255, 255, 0% );");

    QStringList list;
    list << "name for a media.mp3" << "00:00:00" << "artist" << "★★★★★＃";
    AlbumList *album = new AlbumList(this);
    this->addTopLevelItem( album );
    album->addSong(list);
    album->addSong(list);
    album->addSong(list);

    AlbumList *album1 = new AlbumList(this);
    this->addTopLevelItem( album1 );
    for( int i = 0; i < 5; ++i)
        album1->addSong(list);

    AlbumList *album2 = new AlbumList(this);
    this->addTopLevelItem( album2 );
    for( int i = 0; i < 12; ++i)
        album2->addSong(list);

    AlbumList *album3 = new AlbumList(this);
    this->addTopLevelItem( album3 );
    for( int i = 0; i < 10; ++i)
        album3->addSong(list);


    for( int i = 0; i < 40; ++i)
    {
        AlbumList *album4 = new AlbumList(this);
        this->addTopLevelItem( album4 );
        for(int j = 0; j < 10; ++j)
            album4->addSong(list);
    }

// 连接信号槽
    connect(this, SIGNAL(itemPressed(QTreeWidgetItem*,int)),
            this, SLOT(selectedChildren(QTreeWidgetItem*,int)) );
}

// 当选中父节点时，选择其所有子节点
void MediaPlayListView::selectedChildren(QTreeWidgetItem *item, int)
{
    int counts = item->childCount();
    for(int i = 0; i < counts; ++i)
    {
        if( item->child(i)->isSelected() ) continue;
        setItemSelected(item->child(i), true);
    }
}
