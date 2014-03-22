/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:MainWindowEx.h
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

#ifndef MAINWINDOWEX_H
#define MAINWINDOWEX_H

#include <QMainWindow>
#include <QResizeEvent>
#include <QMouseEvent>

namespace Ui {
class MainWindowEx;
}

class MainWindowEx : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowEx(QWidget *parent = 0);
    ~MainWindowEx();

    QMenuBar* mainMenuBar() const;
    QToolBar* mainToolBar() const;
    QStatusBar* mainStatusBar() const;
    void setBackgroundCover(QPixmap &pixmap);

signals:
    void sizeChanged();
    void hoverEnter();
    void hoverLeave();

public slots:
    void showMaximize();

private:
    bool hitTestNCA(MSG *msg, long *result);

protected:
    bool winEvent(MSG *msg, long *result);
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
    void resizeEvent(QResizeEvent *event);

private:
    Ui::MainWindowEx *ui;
    QPixmap           m_backgroundCover;
};

#endif // MAINWINDOWEX_H
