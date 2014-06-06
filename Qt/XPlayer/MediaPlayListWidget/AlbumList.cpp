#include "AlbumList.h"
#include <QDebug>

AlbumList::AlbumList(QTreeWidget *parent) :
    QTreeWidgetItem(parent),
    m_coverWidget(0)
{
    this->setExpanded(true);
    this->setFirstColumnSpanned(true);//合并
    QString artist;
    artist = "Artist---------------------------------------------------"
            "----------------------------------------------------------"
            "------------------------------------";
    this->setText(0, artist);

    m_coverWidget = new QWidget(this->treeWidget());
    this->treeWidget()->setItemWidget(this, 0, m_coverWidget);
    m_coverWidget->setStyleSheet("*{background:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #e7effd, stop: 1 #cbdaf1);}");

//    m_coverWidget->setStyleSheet(
//                "*{background:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
//                "stop: 0 #fafafa, stop: 0.4 #f4f4f4,"
//                "stop: 0.5 #e7e7e7, stop: 1.0 #fafafa);}");

    //add a sapce item
    QTreeWidgetItem *spacer = new QTreeWidgetItem(this);
    spacer->setFirstColumnSpanned(true);//合并
    spacer->setFlags(Qt::NoItemFlags);
    this->addChild(spacer);
}

AlbumList::~AlbumList()
{
}

void AlbumList::addSong(const QStringList &song)
{
    QTreeWidgetItem *songItem = new QTreeWidgetItem;
    songItem->setText( 0, QString::number(this->childCount()) );
    songItem->setText( 1, song.at(0) );
    songItem->setText( 2, song.at(1) );
    songItem->setText( 3, song.at(2) );
    songItem->setText( 4, song.at(3) );
    this->insertChild(childCount()-1, songItem);
//    resizeLastHint();
}

void AlbumList::resizeLastHint()
{
    int counts = childCount();
    QTreeWidgetItem* item = child(counts-1);
    if( !item ) return;

//    qDebug() << counts;
    // 当前行数小于8行，则需要增加高度
    if( 8 - counts > 1 )
    {
        item->setSizeHint( 0, QSize(1, 16*(8-counts)) );
    }
    else
    {
        int h = item->sizeHint(0).height();
        if( h > 16 )
            item->setSizeHint( 0, QSize(1, h/(counts-8)) );
    }
}
