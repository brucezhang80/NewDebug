/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:MetaInfo.cpp
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

#include "MetaInfo.h"

#include <QString>
#include <QByteArray>

MetaInfo::MetaInfo()
{
    this->clear();
}

void MetaInfo::clear()
{
    m_errString = "Unknow error";
    m_content.clear();
    m_content.clear();
    m_infoData.clear();
    m_metaName.clear();
    m_metaSize = 0;
    m_metaType.clear();
}

bool MetaInfo::parse(const QByteArray &data)
{
    this->clear();

    m_content = data;
    if( m_content.isEmpty() )
    {
        m_errString = "Not content.";
        return false;
    }

    return true;
}

QString MetaInfo::errorString() const
{
    return m_errString;
}

QByteArray MetaInfo::content() const
{
    return m_content;
}

QByteArray MetaInfo::infoData() const
{
    return m_infoData;
}

QString MetaInfo::metaName() const
{
    return m_metaName;
}

QString MetaInfo::metaType() const
{
    return m_metaType;
}

qint64 MetaInfo::metaSize() const
{
    return m_metaSize;
}
