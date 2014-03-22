#ifndef SYSTEM_H
#define SYSTEM_H

#include <QRegExp>
#include <QFile>
#include <QDesktopServices>

class System
{
public:
    System();

    static void setAppPath(const QString &path);
    static QString appPath();

    static QString appHomePath();

    static QString lockFile();

    static void setIniPath(const QString &path);
    static QString iniPath();

    static QString translationPath();
    static QString qtTranslationPath();

    static QString soundPath();
    static QString iconPath();

    static QString loginName();
    static QString hostName();
    static QString hostAddress();

    static QString getEnvironmentVariable(const QRegExp &regExp);

    static void setPacketNo(const qint64 &n);
    static QString packetNoString();
    static qint64 packetNo();

    static void setInternalLogFileName( const QString &filePath );
    static void writeInternalLog(const QString &line);

    static QString openUrlProgram();

    static qint64  folderSize( const QString &folder );
    static QString fileCountString(const int &fileCount);
    static QString sizeStringUnit(double size = 0.0, QString sep = "");
    static QString formatSize(qint64 size);
    static QString secondStringUnit(int second);
    static QString bytesToUnitString( const qint64 &bytes );

    static bool createSharedFolder(const QString &path, const QString &netName);// or mac address
    static bool createSharedFolder();
    static QString sharedPath(){ return m_sharedPath; }


    static QString localIPQDocument();

private:
    static void openInternalLogFile();

    static QString  m_appPath;
    static QString  m_iniPath;
    static qint64   m_packetNo;
    static QFile    m_internalLogFile;
    static QString  m_internalLogFileName;
    static QFile    m_qssTheme;
    static QString  m_sharedPath;
};

#endif // SYSTEM_H
