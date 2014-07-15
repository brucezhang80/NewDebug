#include "MediaPlayWidget.h"

#include <QToolButton>
#include <QGridLayout>
#include <QSpacerItem>
#include <QDebug>

//#include <QtMultimedia/QMediaPlayer>

MediaPlayWidget::MediaPlayWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setFixedWidth(180);
    this->setMaximumHeight(66);
//    setAttribute(Qt::WA_TransparentForMouseEvents,true);

//    m_animate = new StateMachineServer;

    QToolButton* prevBtn = new QToolButton;
    prevBtn->setCursor(QCursor(Qt::PointingHandCursor));
    prevBtn->setFixedSize(36, 36);
    prevBtn->setIconSize( QSize(36, 36) );
    prevBtn->setIcon(QIcon(":/images/Button-prev.png"));
    QToolButton* playBtn = new QToolButton;
    playBtn->setCursor(QCursor(Qt::PointingHandCursor));
    playBtn->setFixedSize(50, 50);
    playBtn->setIconSize( QSize(50, 50) );
    playBtn->setIcon(QIcon(":/images/Button-play.png"));
    QToolButton* nextBtn = new QToolButton;
    nextBtn->setCursor(QCursor(Qt::PointingHandCursor));
    nextBtn->setFixedSize(36, 36);
    nextBtn->setIconSize( QSize(36, 36) );
    nextBtn->setIcon(QIcon(":/images/Button-next.png"));

    QGridLayout *gridlayout = new QGridLayout(this);
    gridlayout->setAlignment(Qt::AlignLeft);
//    QSpacerItem* leftSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
//    QSpacerItem* rightSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

//    gridlayout->addItem(leftSpacer, 0, 0, 1, 1);
    gridlayout->addWidget(prevBtn, 0, 0, 2, 1);
    gridlayout->addWidget(playBtn, 0, 1, 2, 1);
    gridlayout->addWidget(nextBtn, 0, 2, 2, 1);
//    gridlayout->addItem(rightSpacer, 0, 3, 1, 1);

    connect( playBtn, SIGNAL(clicked(bool)), this, SLOT(play(bool)) );

//    connect(this, SIGNAL(animateHide()), m_animate, SIGNAL(transHide()) );
//    connect(this, SIGNAL(animateShow()), m_animate, SIGNAL(transShow()) );
}

void MediaPlayWidget::play(bool pause)
{
//    QMediaPlayer* player = new QMediaPlayer();
//    player->setMedia(QUrl::fromLocalFile("E:/Music/心肝宝贝.mp3"));
//    player->setVolume(50);
//    player->play();
    emit playChanged();
}

void MediaPlayWidget::resizeEvent(QResizeEvent *event)
{
//    m_animate->setAnimate(this, StateMachineServer::Bottom);
    QWidget::resizeEvent(event);
}
