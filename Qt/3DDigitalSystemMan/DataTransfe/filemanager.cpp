#include "filemanager.h"

#include <QMessageBox>
#include <QFile>
#include <QFileInfo>
#include<QFileInfoList>
#include <QDir>

#include <QDebug>

FileManager::FileManager(QObject *parent) :
    QThread(parent)
{
    newFile = false;
    verifiedPieces.fill(false);
}

FileManager::~FileManager()
{

}

void FileManager::copy()
{
}

void FileManager::getFolderFiles(QString path, QFileInfoList &fList)
{
    QDir dir(path);
    QFileInfoList list = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files | QDir::NoSymLinks);
    foreach(QFileInfo fInfo, list)
    {
        if(fInfo.isDir())
        {
            getFolderFiles(fInfo.filePath(), fList);
        }
        else
        {
            fList << fInfo;//QDir::toNativeSeparators(fInfo.filePath());
        }
    }
}

bool FileManager::isFile(const QString &src)
{
  QFileInfo file(src);
  if( file.isFile() ) return true;
  return false;
}

bool FileManager::isPath(const QString &src)
{
    QFileInfo file(src);
    if( file.isDir() ) return true;
    return false;
}

void FileManager::copyFolder(QString scrPath, QString dstPath)
{
    QFileInfoList fList;
    fList.clear();
    getFolderFiles(scrPath, fList);
    if(fList.size() == 0)
    {
        //QMessageBox::warning(this, tr("Error"), tr("文件夹是空的:").arg(scrPath), 0, 0);
        qDebug() << tr("文件夹是空的:").arg(srcfileName);
        return;
    }

    if(srcfileName.compare(destinationPath) == 0) return;

    bool stopped = false;
    int curIndex = 0;
    while( !stopped )
    {
        if( curIndex == fList.size() )
        {
            stopped = true;
            continue;
        }
        QFileInfo curFileInfo = fList.at(curIndex);
        QString srcFileName = curFileInfo.filePath();
        QFile src(srcFileName);

        QFileInfo srcTmp(srcfileName);
        QString srcTripname = srcFileName.remove(srcfileName);
        QString dstFileName = srcTripname.insert(0, dstPath+srcTmp.fileName());
        QFile dst(dstFileName);
        QFileInfo tmp(dstFileName);
        qDebug() << tmp.path();
        QDir d(tmp.path());
        if(!d.exists())
            d.mkpath(d.absolutePath());
        //now copy
        if(!src.open(QFile::ReadOnly) || !dst.open(QFile::WriteOnly)) continue;
        fileSize = src.bytesAvailable();
        qint64 BUFF_SIZE = 61440;
        char* buffer = new char[BUFF_SIZE];
        //int oldPercent = 0;
        while( !src.atEnd() )
        {
            dst.write(buffer, src.read(buffer, BUFF_SIZE));
            //dst.flush();
        }
        src.close();
        dst.close();
        delete[] buffer;
        buffer = NULL;

        curIndex++;
        int percent = (curIndex*100) / fList.size();//((curIndex + 1) * 100) / fileSize;
        //if (oldPercent != percent)
        {
            emit verificationProgressSignal(percent);
            //oldPercent = percent;
        }
    }
    fList.clear();
}

void FileManager::copyFile(QString srcFile, QString dstPath)
{
    QFile src(srcFile);
    QFileInfo fileinfo(srcFile);
    QString baseName = fileinfo.fileName();
    QString dstFileName = dstPath + baseName;
    QFile dst(dstFileName);

    //now copy
    if(srcfileName.compare(dstFileName) == 0) return;
    if(!src.open(QFile::ReadOnly) || !dst.open(QFile::WriteOnly))return;
    fileSize = src.bytesAvailable();
    qDebug() << fileSize;
    qint64 BUFF_SIZE = 64*1024;
    char* buffer = new char[BUFF_SIZE];
    int oldPercent = 0;
    qint64 completed = 0;
    while( !src.atEnd() )
    {
        qint64 maxSize = src.read(buffer, BUFF_SIZE);
        dst.write(buffer, maxSize);
        dst.flush();
        completed += BUFF_SIZE;



//        qint64 percent = ((completed + 1) * 100) / fileSize;
//        if (oldPercent != percent)
//        {
//            emit verificationProgressSignal(percent);

//            oldPercent = percent;
//        }
//        emit bytesReceivedSignal(BUFF_SIZE);
    }
    src.close();
    dst.close();
    delete[] buffer;
    buffer = NULL;
}

void FileManager::run()
{
    qDebug() << "run the thread to copy file...";

    if( isPath( srcfileName) )
    {
        //qDebug() << srcfileName << destinationPath;
        copyFolder(srcfileName, destinationPath);
    }
    else
    {
        copyFile(srcfileName, destinationPath);
    }

    qDebug() << "copy done!";

//    fileSize = src.size();
//    int oldPercent = 0;
//    for(qint64 index = 0; index < fileSize; ++index)
//    {
//        dst.write(src.read(index));
//        src.seek(index);
//        dst.seek(index);

//        int percent = ((index + 1) * 100) / fileSize;
//        if (oldPercent != percent)
//        {
//            emit verificationProgressSignal(percent);
//            oldPercent = percent;
//        }
//    }
    //emit verificationDone();
}
