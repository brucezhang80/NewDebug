#include "ThumbnailBar.h"
#include "ThumbnailView.h"

#include <QGridLayout>
#include <QScrollBar>
#include <QEvent>
#include <QWheelEvent>
#include <QDebug>

ThumbnailBar::ThumbnailBar(QWidget *parent) :
    QWidget(parent)
{
    this->setMouseTracking(true);
    this->setFixedHeight(125);

    m_thumbView = new ThumbnailView;
    QGridLayout* grid = new QGridLayout(this);
    grid->addWidget(m_thumbView, 0, 0, 1, 1);
    grid->setMargin(0);
    this->setLayout(grid);

    connect(m_thumbView, SIGNAL(thumbnailChanged(QString)), this, SIGNAL(thumbnailChanged(QString)));
}

void ThumbnailBar::hide()
{
    m_thumbView->hide();
}

void ThumbnailBar::show()
{
    m_thumbView->show();
}

void ThumbnailBar::reset()
{
    m_thumbView->reset();
}

void ThumbnailBar::loadImages(const QString &filename)
{
    m_thumbView->setCurrentFile(filename);
    m_thumbView->setCurrentDir(filename);
    m_thumbView->loadCurrentDir();
    m_thumbView->show();
}

bool ThumbnailBar::event(QEvent *event)
{
    if(event->type() == QEvent::Leave)
    {
        m_thumbView->hide();
    }
    else if(event->type() == QEvent::Enter ||
            event->type() == QEvent::Move)
    {
        m_thumbView->show();
    }

    return QWidget::event(event);
}

void ThumbnailBar::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta();// / 8;
//    int numSteps = numDegrees / 15;

    //if (event->orientation() == Qt::Horizontal)
    {
        QScrollBar* hScroll = m_thumbView->horizontalScrollBar();
        if( hScroll )
        {
            hScroll->setValue(hScroll->value()+numDegrees);
        }
    }
//    else
//    {
//        hScroll->setValue(hScroll->value()+numSteps);
//    }

    event->accept();
}
