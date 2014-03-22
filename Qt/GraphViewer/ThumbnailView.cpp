#include "ThumbnailView.h"
#include "ThumbnailItem.h"

#include <QFileInfo>
#include <QDir>
#include <QIcon>
#include <QTextDocument>
#include <QEvent>
#include <QDebug>

ThumbnailView::ThumbnailView(QWidget *parent) :
    QGraphicsView(parent),
    m_currentDir(""),m_currentFile("")
{
//    this->setMouseTracking(true);
    this->setAlignment(Qt::AlignCenter);
    this->setDragMode(QGraphicsView::ScrollHandDrag);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setFixedHeight(120);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    this->setStyleSheet("background:transparent; border:0");
    m_thumbScene = new QGraphicsScene;
    this->setScene(m_thumbScene);
    m_thumbScene->clear();
}

void ThumbnailView::loadCurrentDir()
{
    reset();

    QFileInfo info(m_currentFile);
    if(info.isFile())
        m_currentDir = info.absoluteDir().absolutePath();
    else
        m_currentDir = m_currentFile;

    // Get QDir instance
    QDir *dir = new QDir(m_currentDir);
    QStringList filters;
    filters << "*.jpg" << "*.png" << "*.bmp" << "*.gif";
    dir->setNameFilters(filters);
    m_imagesInfo = dir->entryInfoList(QDir::NoFilter,QDir::IgnoreCase);
    for(int i = 0; i < m_imagesInfo.length(); ++i)
        m_imagesPath << m_imagesInfo.at(i).absoluteFilePath();

    int spacing = 10;
    int liftup = 10;
    int w = 60;
    int h = 80;

    for(int i = 0; i < m_imagesPath.count(); ++i)
    {
        QPixmap normalPixmap;
        normalPixmap.load(m_imagesPath.at(i));
        normalPixmap = normalPixmap.scaled(w, h, Qt::IgnoreAspectRatio,
                                           Qt::FastTransformation);
        QPixmap hoverPixmap = normalPixmap;

        // 如果这里使用动画效果更好，以后更改为动画机制
        toNormalThumbnail(i, normalPixmap, w, h, spacing, liftup);
        toHoverThumbnail(i, hoverPixmap, w, h, spacing, liftup);

        // Create the pixmapitem, set pixmaps and adjust the position
        ThumbnailItem *thumbItem = new ThumbnailItem;
        connect(thumbItem, SIGNAL(thumbnailClicked(QString)), this, SIGNAL(thumbnailChanged(QString)));
        thumbItem->setThumbnailFilename( m_imagesPath.at(i) );
        thumbItem->setData(1, thumbItem->thumbnailFilename());
        thumbItem->setPixmap(normalPixmap, hoverPixmap);
        thumbItem->setPos( i * (w + spacing), 0);

        // Add to list
        m_thumbItemList.append(thumbItem);
        // And add to scene
        this->m_thumbScene->addItem(thumbItem);
    }

    emit thumbnailChanged(m_imagesPath.at(0));
    // Adjust scene rect
    this->m_thumbScene->setSceneRect(this->m_thumbScene->itemsBoundingRect());
}

void ThumbnailView::reset()
{
    m_thumbScene->clear();
    m_thumbItemList.clear();
    m_imagesPath.clear();
    m_imagesInfo.clear();
}

bool ThumbnailView::compareNamesFileInfo(const QFileInfo &fileinfo1, const QFileInfo &fileinfo2)
{
    const QString s1 = fileinfo1.fileName();
    const QString s2 = fileinfo2.fileName();

    // ignore common prefix..
    int i = 0;

    while ((i < s1.length()) && (i < s2.length()) && (s1.at(i).toLower() == s2.at(i).toLower()))
        ++i;
    ++i;

    // something left to compare?
    if ((i < s1.length()) && (i < s2.length())) {

        // get number prefix from position i - doesnt matter from which string
        int k = i-1;

        //If not number return native comparator
        if(!s1.at(k).isNumber() || !s2.at(k).isNumber()) {

            //Two next lines
            //E.g. 1_... < 12_...
            if(s1.at(k).isNumber())
                return false;
            if(s2.at(k).isNumber())
                return true;
            return QString::compare(s1, s2, Qt::CaseSensitive) < 0;
        }

        QString n = "";
        k--;

        while ((k >= 0) && (s1.at(k).isNumber())) {
            n = s1.at(k)+n;
            --k;
        }

        // get relevant/signficant number string for s1
        k = i-1;
        QString n1 = "";
        while ((k < s1.length()) && (s1.at(k).isNumber())) {
            n1 += s1.at(k);
            ++k;
        }

        // get relevant/signficant number string for s2
        //Decrease by
        k = i-1;
        QString n2 = "";
        while ((k < s2.length()) && (s2.at(k).isNumber())) {
            n2 += s2.at(k);
            ++k;
        }

        // got two numbers to compare?
        if (!n1.isEmpty() && !n2.isEmpty())
            return (n+n1).toInt() < (n+n2).toInt();
        else {

            // not a number has to win over a number.. number could have ended earlier... same prefix..
            if (!n1.isEmpty())
                return false;
            if (!n2.isEmpty())
                return true;
            return s1.at(i) < s2.at(i);
        }
    } else {
        // shortest string wins
        return s1.length() < s2.length();
    }
}


void ThumbnailView::toNormalThumbnail(int index, QPixmap &normal, int width, int height,
                                    int spacing, int liftUp)
{
    // The normal image pixmap
    QPixmap normalPixmap( width , height+liftUp);
    normalPixmap.fill(Qt::transparent);
    QPainter paint(&normalPixmap);
    paint.setCompositionMode(QPainter::CompositionMode_SourceOver);
    QPen pen(Qt::white);
    paint.setPen(pen);
    paint.fillRect(0, liftUp, width , height+liftUp, QColor::fromRgba(qRgba(0,0,0,100)));
    paint.drawPixmap(spacing/3, liftUp, width, height, normal);

    QTextDocument txt;
    txt.setHtml("<center><div style=\"text-align: center; font-size: 8pt; font-wight: bold; color: white; background: rgba(0,0,0,80); border-radius: 10px;\">" + m_imagesInfo.at(index).fileName() + "</div></center>");
    txt.setTextWidth( width );
    paint.translate(3, width*2/3.0);
    txt.drawContents(&paint);
    paint.end();
    normal = normalPixmap;
}

void ThumbnailView::toHoverThumbnail(int index, QPixmap &hover, int width, int height,
                                   int spacing, int liftUp)
{
    // The hover image pixmap
    QPixmap hoverPixmap( width, height);
    hoverPixmap.fill(Qt::transparent);
    QPainter paint(&hoverPixmap);
    paint.setCompositionMode(QPainter::CompositionMode_SourceOver);
    QPen pen(Qt::white);
    paint.setPen(pen);
    paint.fillRect(0, 0, width, height+liftUp, QColor::fromRgba(qRgba(0,0,0,150)));
    paint.drawPixmap(spacing/3, 0, width, height, hover);

    QTextDocument txt;
    txt.setHtml("<center><div style=\"text-align: center; font-size: 8pt; font-wight: bold; color: white; background: rgba(0,0,0,0); border-radius: 10px;\">" + m_imagesInfo.at(index).fileName() + "</div></center>");
    txt.setTextWidth( width );
    paint.translate(3, width*2/3.0);
    txt.drawContents(&paint);
    paint.end();

    hover = hoverPixmap;
}
