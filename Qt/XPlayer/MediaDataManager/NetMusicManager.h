/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:NetMusicManager.h
**   Author: Young Yori
**   Data: Administrator 2014/2/19 2014
**   Description:
**          This file is part of the Data Development Department Project and shall
**      only be used, modified, and distributed under the terms of the Data
**      Development Department this Source.
**
**      Included in the build system of the FreeType library.
**--------------------------------------------------------------------------------
**                      www.newdebug.com, 12319597@qq.com
**************************************************************************/

#ifndef NETMUSICMANAGER_H
#define NETMUSICMANAGER_H

#include <QObject>
#include <QStringList>
#include "DataManager.h"

class QNetworkAccessManager;
class QNetworkReply;
class QEventLoop;
class QByteArray;
class QThread;
class QPixmap;
class QByteArray;
class QXmlStreamReader;

class NetMusicManager : public DataManager
{
    Q_OBJECT
public:
    enum MusicEntryAPI
    {
        QQMusic = 0,
        BaiduMusic,
        KugoukMusic,
        DuomiMusic
    };

    explicit NetMusicManager(QObject *parent = 0);
    ~NetMusicManager();


signals:

public slots:
    void searchMusic(const QString &key);

private slots:
    void replyReadyRead();

private:
    QString configFilename() const;
    bool readConfig( const QString &filename );
    void readConfigAPI(const int &id, const QString &value);
    QString makeSearchUrl(const QString &key);
    QString musicAPI(const MusicEntryAPI &API = BaiduMusic) const;
    void setMusicAPI(const QString &urlAPI, const MusicEntryAPI &API);

private:
    QString     m_qqAPI;
    QString     m_baiduAPI;
    QString     m_kugouAPI;
    QString     m_duomiAPI;

    QString     m_configFilename;
    QStringList m_entryNames;

    QNetworkAccessManager*      m_netManager;
    QNetworkReply*              m_netReply;
};

#endif // NETMUSICMANAGER_H
