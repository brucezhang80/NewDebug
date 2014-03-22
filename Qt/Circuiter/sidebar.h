#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <QtGui>
#include "ui_mainwindow.h"

class SideBar : public QWidget,
{
    Q_OBJECT
public:
    explicit SideBar(QWidget *parent = 0);
    void initSideBar();

signals:
    
public slots:
    
private:
    QToolButton      *m_jobSender;
    QToolButton      *m_jobReciever;
    QToolButton      *m_jobHistory;
    QToolButton      *m_jobInStorage;
    QToolButton      *m_jobStatus;
    QToolButton      *m_personManager;
    QToolButton      *m_otherActions;
    QToolButton      *m_settings;

    QList<QToolButton*> m_btnList;

};

#endif // SIDEBAR_H
