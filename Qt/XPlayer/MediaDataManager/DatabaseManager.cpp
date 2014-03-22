/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:MediaDataManager.cpp
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

#include "DatabaseManager.h"
#include <QStringList>

DatabaseManager::DatabaseManager(QObject *parent) :
    QObject(parent)
{
}

bool DatabaseManager::createConnect()
{
    switch( m_connState )
    {
    case Read:
        {
            if(QSqlDatabase::contains("READ"))
                m_db = &QSqlDatabase::database("READ");
            else
            {
                m_db = &QSqlDatabase::addDatabase("QSQLITE", "READ");
                m_db->setDatabaseName("");
            }
        };
        break;
    case Write:
        {
            if(QSqlDatabase::contains("WRITE"))
                m_db = &QSqlDatabase::database("WRITE");
            else
            {
                m_db = &QSqlDatabase::addDatabase("QSQLITE", "WRITE");
                m_db->setDatabaseName("");
            }
        };
        break;
    }

    return true;
}

bool DatabaseManager::closeConnect()
{
    QStringList names = m_db->connectionNames();

    m_db->close();
    delete m_db;
    m_db = NULL;

    foreach (QString name, names)
    {
        QSqlDatabase::removeDatabase(name);
    }
    return true;
}

bool DatabaseManager::createTable()
{

    return true;
}
