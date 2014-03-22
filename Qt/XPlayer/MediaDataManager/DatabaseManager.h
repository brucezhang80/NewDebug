/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:MediaDataManager.h
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

#ifndef MEDIADATAMANAGER_H
#define MEDIADATAMANAGER_H

#include <QObject>
#include <QSqlDatabase>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    enum ConnectState
    {
        None = 0,
        Read,
        Write
    };

    explicit DatabaseManager(QObject *parent = 0);

    bool createConnect();
    bool closeConnect();

    bool createTable();

    inline QSqlDatabase* database() const { return m_db; }

    void setConnName(const QString &name);
    QString curConnName() const;

    ConnectState connState() const { return m_connState; }
    void setConnState(ConnectState state = Read) { m_connState = state; }

signals:

public slots:

private:
    QSqlDatabase*   m_db;
    QString         m_connName;
    ConnectState    m_connState;
};

#endif // MEDIADATAMANAGER_H
