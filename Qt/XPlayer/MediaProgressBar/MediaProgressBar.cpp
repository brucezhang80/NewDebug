#include "MediaProgressBar.h"
#include <QHBoxLayout>

MediaProgressBar::MediaProgressBar(QWidget *parent) :
    QProgressBar(parent)
{
    m_animate = new StateMachineServer;

    this->setMaximumHeight(30);
    this->setContentsMargins(0,0,0,0);

    this->setMaximumHeight(6);
    this->setTextVisible(false);
    this->setAlignment(Qt::AlignLeft);
    this->setValue(40);
//    this->setFormat("00:00:00");
    connect(this, SIGNAL(animateHide()), m_animate, SIGNAL(transHide()) );
    connect(this, SIGNAL(animateShow()), m_animate, SIGNAL(transShow()) );
}

void MediaProgressBar::resizeEvent(QResizeEvent *event)
{
    m_animate->setAnimate(this, StateMachineServer::Bottom);
    QWidget::resizeEvent(event);
}
