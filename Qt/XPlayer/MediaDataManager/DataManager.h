/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:DataManager.h
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

#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QVector>

class QFile;

class DataManager : public QObject
{
    Q_OBJECT
public:
    explicit DataManager(QObject *parent = 0);
    virtual ~DataManager();

    virtual int index();

    QVector<QFile*> files() const;
    QString fileName(int index) const;
    QString fileType(int index) const;
    qint64 fileSize(int index) const;
    qint32 count() const;

    /*delete file from local disk*/
    bool deleteFile(int index);

    /*just remove from list, not remove file from local disk*/
    bool removeFile(int index);
    bool renameFile(int index, const QString &newName);
    bool renameFile(int index, const QString &newName, const QString &oldName);

signals:

public slots:

private:
    int             m_index;
    QVector<QFile*>  m_fileVec;
};

#endif // DATAMANAGER_H
