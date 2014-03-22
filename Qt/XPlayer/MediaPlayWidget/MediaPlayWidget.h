#ifndef MEDIAPLAYWIDGET_H
#define MEDIAPLAYWIDGET_H

#include <QWidget>
#include "StateMachineServer/StateMachineServer.h"

class MediaPlayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MediaPlayWidget(QWidget *parent = 0);

signals:
    void playChanged();
    void animateHide();
    void animateShow();

public slots:
    void play(bool pause);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    StateMachineServer*      m_animate;
};

#endif // MEDIAPLAYWIDGET_H
