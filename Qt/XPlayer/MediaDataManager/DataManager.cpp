/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:DataManager.cpp
**   Author: Young Yori
**   Data: Administrator 2014/2/20 2014
**   Description:
**          This file is part of the Data Development Department Project and shall
**      only be used, modified, and distributed under the terms of the Data
**      Development Department this Source.
**
**      Included in the build system of the FreeType library.
**--------------------------------------------------------------------------------
**                      www.newdebug.com, 12319597@qq.com
**************************************************************************/

#include "DataManager.h"

#include <QFile>
#include <QFileInfo>

#include <QDebug>


DataManager::DataManager(QObject *parent) :
    QObject(parent)
{
    m_index = -1;
}

DataManager::~DataManager()
{
    m_fileVec.clear();
}

int DataManager::index()
{
    return m_index;
}

QVector<QFile*> DataManager::files() const
{
    return m_fileVec;
}

QString DataManager::fileName(int index) const
{
    return m_fileVec.at(index)->fileName();
}

QString DataManager::fileType(int index) const
{
    QFileInfo info( fileName(index) );
    return info.suffix();
}

qint64 DataManager::fileSize(int index) const
{
    return m_fileVec.at(index)->size();
}

qint32 DataManager::count() const
{
    return m_fileVec.size();
}

bool DataManager::deleteFile(int index)
{
    QFile* file = m_fileVec.at(index);
    if( file && file->remove() )
        return true;
    return false;
}

bool DataManager::removeFile(int index)
{
    QFile* file = m_fileVec.at(index);
    if( file )
    {
        m_fileVec.remove(index);
        return true;
    }
    return false;
}

bool DataManager::renameFile(int index, const QString &newName)
{
    QFile* file = m_fileVec.at(index);
    if( file )
    {
        file->rename(newName);
        return true;
    }
    return false;
}

bool DataManager::renameFile(int index, const QString &newName, const QString &oldName)
{
    QFile* file = m_fileVec.at(index);
    if( file )
    {
        file->rename(oldName, newName);
        return true;
    }
    return false;
}
