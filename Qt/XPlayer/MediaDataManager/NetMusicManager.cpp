/**************************************************************************
** Copyright (C) 2013 Southgis Co.,Ltd. Written by Young Yori, All rights reserved.
**--------------------------------------------------------------------------------
**   Filename:NetMusicManager.cpp
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

#include "NetMusicManager.h"

//#include "Json.h"

#include <QNetworkAccessManager>
#include <QDir>
#include <QByteArray>
#include <QThread>
#include <QEventLoop>
#include <QNetworkReply>
#include <QTcpSocket>
#include <QPixmap>
#include <QDateTime>
#include <QXmlStreamReader>
#include <QTextCodec>

#include <QDebug>


NetMusicManager::NetMusicManager(QObject *parent) :
    DataManager(parent)
{
    m_configFilename = "./data/urlConfig.xml";
    readConfig(m_configFilename);

    m_netManager = new QNetworkAccessManager();
}

NetMusicManager::~NetMusicManager()
{

}

QString NetMusicManager::musicAPI(const NetMusicManager::MusicEntryAPI &API) const
{
    QString urlApi = "";
    switch (API)
    {

    case QQMusic:
        urlApi = m_qqAPI;
        break;
    case BaiduMusic:
        urlApi = m_baiduAPI;
        break;
    case KugoukMusic:
        urlApi = m_kugouAPI;
        break;
    case DuomiMusic:
        urlApi = m_duomiAPI;
        break;

    default:
        urlApi = m_baiduAPI;
        break;
    }

    return urlApi;
}

void NetMusicManager::setMusicAPI(const QString &urlAPI, const NetMusicManager::MusicEntryAPI &API)
{
    switch (API)
    {
    case QQMusic:
        m_qqAPI = urlAPI;
        break;
    case BaiduMusic:
        m_baiduAPI = urlAPI;
        break;
    case KugoukMusic:
        m_kugouAPI = urlAPI;
        break;
    case DuomiMusic:
        m_duomiAPI = urlAPI;
        break;
    default:
        break;
    }
}

void NetMusicManager::searchMusic(const QString &key)
{
    QString urlString = makeSearchUrl(key);
    qDebug() << "searchinging: " << urlString;

    QUrl url(urlString);
    QNetworkRequest request;
    request.setUrl(url);
    m_netReply = m_netManager->get(request);
//    connect(request, SIGNAL(finished()), this, SLOT(downloadFinished()));
    connect(m_netReply, SIGNAL(readyRead()), this, SLOT(replyReadyRead()));
}

void NetMusicManager::replyReadyRead()
{
    qDebug() << "search done.";
    if( m_netReply->error() == QNetworkReply::NoError )
    {
        QString data =  QString::fromLocal8Bit( m_netReply->readAll() );
        qDebug() << data;
    }
    else
    {
        qDebug() << m_netReply->errorString();
    }
}

QString NetMusicManager::configFilename() const
{
    return m_configFilename;
}

bool NetMusicManager::readConfig(const QString &filename)
{
    QFile file(filename);
    if( file.open(QFile::ReadOnly | QFile::Text) )
    {
        QXmlStreamReader xml(&file);
        while( !xml.atEnd() )
        {
            xml.readNext();
            if( xml.isStartElement() )
            {
                if( xml.name().toString() == "Entry")
                {
                    int id = xml.attributes().value("id").toString().toInt();
                    QString name = xml.attributes().value("name").toString();
                    QString url = xml.attributes().value("value").toString();
                    readConfigAPI(id, url);
                }
            }
            if( xml.hasError() )
            {
                qDebug() << "ERROR: " << xml.errorString();
            }
        }

        file.close();
    }
    else
    {
        qDebug() << "Read config file failed.";
    }

    return false;
}

void NetMusicManager::readConfigAPI(const int &id, const QString &value)
{
    switch (id)
    {
    case 0:
        setMusicAPI(value, NetMusicManager::QQMusic);
        break;
    case 1:
        setMusicAPI(value, NetMusicManager::BaiduMusic);
        break;
    case 2:
        setMusicAPI(value, NetMusicManager::KugoukMusic);
        break;
    case 3:
        setMusicAPI(value, NetMusicManager::DuomiMusic);
        break;

    default:
        setMusicAPI(value, NetMusicManager::BaiduMusic);
        break;
    }
}

QString NetMusicManager::makeSearchUrl(const QString &key)
{
    QString resultUrl = "";
    if( key == "")
        resultUrl = "http://music.baidu.com/top/new";
    else
        resultUrl = m_baiduAPI.replace("{0}", key);

    qDebug() << resultUrl;
    return resultUrl;
}
