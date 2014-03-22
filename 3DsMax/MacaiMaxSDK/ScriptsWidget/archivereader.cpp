#include "archivereader.h"
#include "zip.h"
#include "unzip.h"

#include <QObject>
#include <QFile>
#include <QDir>
#include <QMessageBox>

#include <QDebug>

/*
    ѹ�������ݸ�ʽ����Ϊ (/�������ļ���/�ļ�s)
*/
ArchiveReader::ArchiveReader()
{
}

// ���̫����
QString ArchiveReader::readArchiveFile(const QString &title, const QString &archiveFile, const QString &password)
{
    QString data;
    if( title.isEmpty() )
        return data;
    if ( !QFile::exists(archiveFile) )
    {
        qDebug() << "File does not exist.";
        QMessageBox::information(0, title, "File does not exist:" + title, 0);
        return data;
    }

    UnZip::ErrorCode ec;
    UnZip uz;

    if ( !password.isEmpty() )
        uz.setPassword(password);

    ec = uz.openArchive( archiveFile );
    if ( ec != UnZip::Ok )
    {
        qDebug() << "Unable to open archive: " << uz.formatError(ec);
        QString err = "Unable to open archive: " + uz.formatError(ec);
        QMessageBox::information(0, title, err, 0);
        return data;
    }

    QList<UnZip::ZipEntry> list = uz.entryList();
    if ( !list.isEmpty() )
    {
        for (int i = 0; i < list.size(); ++i)
        {
            const UnZip::ZipEntry& entry = list.at(i);

            QString file = entry.filename;

            // ֻȡ�ļ���,ȥ���ļ�����
            QString basename = file;
            int idx = file.lastIndexOf("/");    //idx==file.length()-1 ���ļ���
            if (idx >= 0 && idx != file.length()-1)
            {
                basename = file.right(file.length() - idx - 1);
            }
            if( basename == title/*.contains( title, Qt::CaseSensitive) || title.contains( file, Qt::CaseSensitive)*/ )
            {
                QString tempDir = QDir::tempPath();
                if ( UnZip::Ok == uz.extractFile(file, tempDir) )
                {
                    QString tempFile = tempDir + "/" + file;
                    data = tempFile;
                }
            }
        }
    }
    else
    {
        QMessageBox::information(0, title, "empty", 0);
    }

    uz.closeArchive();
    return data;
}

QStringList ArchiveReader::listFiles(const QString &archiveFile, const QString &password)
{
    QStringList fileList;
    if ( !QFile::exists(archiveFile) )
    {
        qDebug() << "File does not exist.";
        QString err = QObject::tr("File does not exist: ") + archiveFile;
        QMessageBox::information(0, QObject::tr("error"), err, QMessageBox::Ok);
        return fileList;
    }

    UnZip::ErrorCode ec;
    UnZip uz;

    if ( !password.isEmpty() )
        uz.setPassword(password);

    ec = uz.openArchive( archiveFile );
    if ( ec != UnZip::Ok )
    {
        qDebug() << "Unable to open archive: " << uz.formatError(ec);
        QString err = QObject::tr("Unable to open archive: ") + uz.formatError(ec);
        QMessageBox::information(0, QObject::tr("error"), err, QMessageBox::Ok);
        return fileList;
    }

    QList<UnZip::ZipEntry> list = uz.entryList();
    if ( !list.isEmpty() )
    {
        for (int i = 0; i < list.size(); ++i)
        {
            const UnZip::ZipEntry& entry = list.at(i);
            QString file = entry.filename;
//            // ֻȡ�ļ���,ȥ���ļ�����
//            int idx = file.lastIndexOf("/");    //idx==file.length()-1 ���ļ���
//            if (idx >= 0 && idx != file.length()-1)
//            {
//                file = file.right(file.length() - idx - 1);
//            }
//            if( idx == file.length() )
//                continue;
            fileList.append( file );
        }
    }

    uz.closeArchive();
    return fileList;
}
