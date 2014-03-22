/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:MetaInfo.h
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

#ifndef METAINFO_H
#define METAINFO_H

#include <QOBject>

class QString;
class QByteArray;

class MetaInfo
{
public:
    MetaInfo();

    void clear();
    bool parse(const QByteArray &data);

    QString errorString() const;
    QByteArray content() const;
    QByteArray infoData() const;

    QString metaName() const;
    QString metaType() const;
    qint64 metaSize() const;

private:
    QString     m_errString;
    QByteArray  m_content;
    QByteArray  m_infoData;

    QString     m_metaName;
    QString     m_metaType;
    qint64      m_metaSize;

};

#endif // METAINFO_H
