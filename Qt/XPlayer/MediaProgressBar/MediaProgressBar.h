#ifndef MEDIAPROGRESSBAR_H
#define MEDIAPROGRESSBAR_H

//#include <QWidget>
#include <QProgressBar>
//#include <QLabel>
//#include "StateMachineServer/StateMachineServer.h"

class MediaProgressBar : public QProgressBar
{
    Q_OBJECT
public:
    explicit MediaProgressBar(QWidget *parent = 0);

signals:
    void animateHide();
    void animateShow();

public slots:

protected:
    void resizeEvent(QResizeEvent *event);

private:
//    QProgressBar*       m_progressBar;
//    QLabel*             m_timeLeft;
//    StateMachineServer*      m_animate;
};

#endif // MEDIAPROGRESSBAR_H
