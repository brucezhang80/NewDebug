#include "system.h"
#include "constants.h"

#include <QLibraryInfo>
#include <QFile>
#include <QDir>
#include <QProcess>
#include <QTextStream>
#include <QHostAddress>
#include <QHostInfo>
#include <QDebug>

#include <windows.h>
#include <lm.h>
#include <AccCtrl.h>
#include <Aclapi.h>
#include <atlconv.h>// to unicode

QString System::m_iniPath;
QString System::m_appPath;
qint64 System::m_packetNo;
QFile System::m_internalLogFile;
QString System::m_internalLogFileName;
QFile System::m_qssTheme;
QString System::m_sharedPath;

static BOOL AddAccessRights(TCHAR *lpszFileName, TCHAR *lpszAccountName,DWORD dwAccessMask);


System::System()
{
}

void System::setAppPath(const QString &path)
{
    m_appPath = path;
}

QString System::appPath()
{
    return m_appPath;
}

QString System::appHomePath()
{
    return QDir::homePath() + "/.ipq";
}

QString System::lockFile()
{
    return appHomePath() + "/ipq.lock";
}

void System::setIniPath(const QString &path)
{
    m_iniPath = path;
}

QString System::iniPath()
{
    if (!m_iniPath.isEmpty())
    {
        return m_iniPath;
    }
    else
    {
        if (QFile::exists(appHomePath()))
        {
            return appHomePath();
        }
    }

    return "";

}

QString System::translationPath()
{
#ifdef TRANSLATION_PATH
    QString path = QString(TRANSLATION_PATH);
    if (!path.isEmpty()) {
        return path;
    } else {
        return appPath() + "/translations";
    }
#else
    return appPath() + "/translations";
#endif
}

QString System::qtTranslationPath()
{
    return QLibraryInfo::location(QLibraryInfo::TranslationsPath);
}

QString System::soundPath()
{
#ifdef SOUND_PATH
    QString path = QString(SOUND_PATH);
    if (!path.isEmpty()) {
        return path;
    } else {
        return m_appPath + "/sounds";
    }
#else
    return m_appPath + "/sounds";
#endif
}

QString System::iconPath()
{
#ifdef ICON_PATH
    QString path = QString(ICON_PATH);
    if (!path.isEmpty()) {
        return path;
    } else {
        return m_appPath + "/icons";
    }
#else
    return m_appPath + "/icons";
#endif
}

QString System::loginName()
{
    return getEnvironmentVariable(QRegExp("USERNAME.*"));
}

QString System::hostName()
{
    QString name;

    name = QHostInfo::localHostName();
    if (name.isEmpty())
    {
        QStringList envVariables;
        envVariables << "USERNAME.*" << "USER.*" << "USERDOMAIN.*"
                     << "HOSTNAME.*" << "DOMAINNAME.*";

        QStringList environment = QProcess::systemEnvironment();
        foreach (QString string, envVariables)
        {
            int index = environment.indexOf(QRegExp(string));
            if (index != -1)
            {
                QStringList stringList = environment.at(index).split('=');
                if (stringList.size() == 2)
                {
                    name = stringList.at(1).toUtf8();
                    break;
                }
            }
        }
    }

    return name;
}

QString System::hostAddress()
{
    QString localhostname = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(localhostname);
    //只需要ipv4的IP地址
    foreach(QHostAddress address, info.addresses())
    {
        if( address.protocol() == QAbstractSocket::IPv4Protocol)
        {
            return address.toString();
        }
    }

    return ("00:00:00:00");
}

QString System::getEnvironmentVariable(const QRegExp &regExp)
{
    QStringList environment = QProcess::systemEnvironment();

    int index = environment.indexOf(regExp);
    if (index != -1)
    {
        QStringList sl = environment.at(index).split("=");
        if (sl.size() == 2)
        {
            return sl.at(1).toUtf8();
        }
    }

    return QString("");
}

void System::setPacketNo(const qint64 &n)
{
    m_packetNo = n;
}

QString System::packetNoString()
{
    ++m_packetNo;
    return QString("%1").arg(m_packetNo);
}

qint64 System::packetNo()
{
    ++m_packetNo;
    return m_packetNo;
}

void System::setInternalLogFileName(const QString &filePath)
{
    m_internalLogFileName = filePath;
}

void System::writeInternalLog(const QString &line)
{
    openInternalLogFile();
    QTextStream ts(&m_internalLogFile);

    ts << line << endl;
}

QString System::openUrlProgram()
{
#if 0
    return appPath() + "/ipq-xdg-open";
#else
    return "xdg-open";
#endif
}

qint64 System::folderSize(const QString &folder)
{
    QDir *dir = new QDir(folder);
    QStringList filter;
    QList<QFileInfo> *fileInfo=new QList<QFileInfo>( dir->entryInfoList(filter) );
    qint64 _size = 0;
    for ( int i = 0; i < fileInfo->size(); ++i )
    {
        _size +=  fileInfo->at(i).size();
    }

    return _size;
}

QString System::fileCountString(const int &fileCount)
{
    if (fileCount <= 1)
    {
        return QString(QObject::tr("%1 file")).arg(fileCount);
    }
    else
    {
        return QString(QObject::tr("%1 files")).arg(fileCount);
    }

    // 这里是不会被执行的
    return QString();
}

QString System::sizeStringUnit(double size, QString sep)
{
    QString s;
    if (size >= ONE_MB)
    {
        s = QString("%1%2MB").arg(size/ONE_MB, 0, 'f', 1).arg(sep);
    }
    else
    {
        s = QString("%1%2KB").arg(qMax(size/ONE_KB, 1.0), 0, 'f', 0).arg(sep);
    }

    return s;
}

QString System::secondStringUnit(int second)
{
    if (second <= 1)
    {
        return QString(QObject::tr("%1 second")).arg(second);
    }
    else
    {
        return QString(QObject::tr("%1 seconds")).arg(second);
    }

    // this should never be reached
    return QString();
}

QString System::bytesToUnitString(const qint64 &bytes)
{
    qint32 B = 1; //byte
    qint32 KB = 1024 * B; //kilobyte
    qint32 MB = 1024 * KB; //megabyte
    qint64 GB = 1024 * MB; //gigabyte
    qint64 TB = 1024 * GB;//terabyte

    QString sizeString = ("0");
    if ((bytes >= KB) && ( bytes < MB))
    {
        sizeString = QString("%1 KB").arg( (double)bytes / KB, 0, 'f', 1 );
    }
    else if ((bytes >= MB) && (bytes < GB))
    {
        sizeString = QString("%1 MB").arg( (double)bytes / MB, 0, 'f', 1 );
    }
    else if ((bytes >= GB) && (bytes < TB))
    {
       sizeString = QString("%1 GB").arg( (double)bytes / GB, 0, 'f', 1 );
    }
    else if (bytes >= TB)
    {
        sizeString = QString("%1 TB").arg( (double)bytes / TB, 0, 'f', 1 );
    }
    else
    {
        sizeString = QString("%1 B").arg((double)bytes, 0, 'f', 1);
    }

    return sizeString;
}

QString System::formatSize(qint64 size)
{
    QString str = QString("%1 B").arg(size);
    int base = 1024;
    qint64 mult = 1;
    for (int i = 0; i < 4; i++)
    {
        qint64 div = size / mult;
        if (div < base)
        {
            double val = (double)size / mult;
            switch (i)
            {
                case 1: str = QString("%1Kb").arg(val, 0, 'f', 1); break;
                case 2: str = QString("%1Mb").arg(val, 0, 'f', 1); break;
                case 3: str = QString("%1Gb").arg(val, 0, 'f', 1); break;
                default: break;
            }
            break;
        }
        mult *= base;
    }
    return str;
}

void System::openInternalLogFile()
{
    if (!m_internalLogFile.isOpen())
    {
        m_internalLogFile.setFileName(m_internalLogFileName);
        m_internalLogFile.open(QIODevice::WriteOnly);
    }
}

// create shared folder for copy data.
// shared folder path: Driver + appName_netName(hostName)
bool System::createSharedFolder(const QString &path, const QString &netName)
{
    NET_API_STATUS res;
    SHARE_INFO_2 p;
    DWORD parm_err = 0;

    QDir dir(path);
    if( !dir.exists() )
        dir.mkpath(path);

    m_sharedPath = path;
    p.shi2_netname = (LPWSTR)netName.utf16();
    p.shi2_type = STYPE_DISKTREE; // disk drive
    p.shi2_remark = TEXT("TESTSHARE to test NetShareAdd");
    p.shi2_permissions = 0;
    p.shi2_max_uses = 4;
    p.shi2_current_uses = 0;
    p.shi2_path = (LPWSTR)path.utf16();
    p.shi2_passwd = NULL; // no password

    res = NetShareAdd(NULL, 2, (LPBYTE) &p, &parm_err);

    if(res == 0)
       qDebug("Share created.\n");
    else
        qDebug("Error: %u\tparmerr=%u\n", res, parm_err);

    return (res == 0 || res == 2118); // 2118表示已经存在的共享
}

bool System::createSharedFolder()
{

    NET_API_STATUS res;
    SHARE_INFO_2 p;
    DWORD parm_err = 0;

    QString netName = AppNamePre + loginName();
    QString path = Driver + QDir::separator() + netName;
    QDir dir(path);
    if( !dir.exists() )
        dir.mkpath(path);
    m_sharedPath = path;


    // argv[1] – 文件（目录）名
    // argv[2] – 用户（组）名
    // GENERIC_ALL表示所有的权限，其是一系列的 NTFS 权限的或
    //   NTFS的文件权限很细，还请参看 MSDN 。
    if (!AddAccessRights((wchar_t*)m_sharedPath.utf16(), L"Everyone", GENERIC_ALL))
    {
        qDebug("AddAccessRights() failed.\n");
//        return 1;
    }
    else
    {
        qDebug("AddAccessRights() succeeded.\n");
//        return 0;
    }

    p.shi2_netname = (LPWSTR)netName.unicode();
    p.shi2_type = STYPE_DISKTREE; // disk drive
    p.shi2_remark = NULL;
    p.shi2_permissions = ACCESS_ALL;
    p.shi2_max_uses = -1;
    p.shi2_current_uses = 0;
    p.shi2_path = (LPWSTR)path.unicode();
    p.shi2_passwd = NULL; // no password


    res = NetShareAdd(NULL, 2, (LPBYTE) &p, &parm_err);


    if(res == 0)
       qDebug("Share created.\n");
    else
        qDebug("Error: %u\tparmerr=%u\n", res, parm_err);

    return (res == 0 || res == 2118); // 2118表示已经存在的共享
    //    return true;
}

QString System::localIPQDocument()
{
    QString userDoc = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
    QString docPath = userDoc + QDir::separator() + Program + QDir::separator();
    QDir dir(docPath);
    if( !dir.exists() )
        dir.mkpath(docPath);

    return docPath;
}

//使用 Windows 的 HeapAlloc 函数进行动态内存分配
#define myheapalloc(x) (HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, x))
#define myheapfree(x) (HeapFree(GetProcessHeap(), 0, x))
typedef BOOL (WINAPI *SetSecurityDescriptorControlFnPtr)(
  IN PSECURITY_DESCRIPTOR pSecurityDescriptor,
  IN SECURITY_DESCRIPTOR_CONTROL ControlBitsOfInterest,
  IN SECURITY_DESCRIPTOR_CONTROL ControlBitsToSet);
typedef BOOL (WINAPI *AddAccessAllowedAceExFnPtr)(PACL pAcl,DWORD dwAceRevision,DWORD AceFlags,DWORD AccessMask,PSID pSid);

// 增加共享文件夹的访问权限
BOOL AddAccessRights(TCHAR *lpszFileName, TCHAR *lpszAccountName,DWORD dwAccessMask)
{
  // 声明 SID 变量
  SID_NAME_USE  snuType;
  // 声明和 LookupAccountName 相关的变量（注意，全为 0 ，要在程序中动态分配）
  TCHAR *    szDomain    = NULL;
  DWORD     cbDomain    = 0;
  LPVOID     pUserSID    = NULL;
  DWORD     cbUserSID   = 0;
  // 和文件相关的安全描述符  SD  的变量
  PSECURITY_DESCRIPTOR pFileSD = NULL;   // 结构变量
  DWORD     cbFileSD    = 0;    // SD的 size
  // 一个新的 SD 的变量，用于构造新的 ACL （把已有的 ACL 和需要新加的 ACL 整合起来）
  SECURITY_DESCRIPTOR newSD;
  // 和 ACL  相关的变量
  PACL      pACL      = NULL;
  BOOL      fDaclPresent;
  BOOL      fDaclDefaulted;
  ACL_SIZE_INFORMATION AclInfo;
  // 一个新的  ACL  变量
  PACL      pNewACL    = NULL; //结构指针变量
  DWORD     cbNewACL    = 0;   //ACL的 size
  // 一个临时使用的  ACE  变量
  LPVOID     pTempAce    = NULL;
  UINT      CurrentAceIndex = 0; //ACE在 ACL 中的位置
  UINT      newAceIndex = 0; //新添的 ACE 在 ACL 中的位置
  //API函数的返回值，假设所有的函数都返回失败。
  BOOL      fResult;
  BOOL      fAPISuccess;
  SECURITY_INFORMATION secInfo = DACL_SECURITY_INFORMATION;
  // 下面的两个函数是新的 API 函数，仅在 Windows 2000 以上版本的操作系统支持。
  // 在此将从 Advapi32.dll 文件中动态载入。如果你使用 VC++ 6.0 编译程序，而且你想
  // 使用这两个函数的静态链接。则请为你的编译加上： /D_WIN32_WINNT=0x0500
  // 的编译参数。并且确保你的 SDK 的头文件和 lib 文件是最新的。
  SetSecurityDescriptorControlFnPtr _SetSecurityDescriptorControl = NULL;
  AddAccessAllowedAceExFnPtr _AddAccessAllowedAceEx = NULL;
  __try
    {
        //
        // STEP 1: 通过用户名取得 SID
        // 在这一步中 LookupAccountName 函数被调用了两次，第一次是取出所需要
        // 的内存的大小，然后，进行内存分配。第二次调用才是取得了用户的帐户信息。
        // LookupAccountName同样可以取得域用户或是用户组的信息。（请参看 MSDN ）
        //
        fAPISuccess = LookupAccountName(NULL, lpszAccountName,
        pUserSID, &cbUserSID, szDomain, &cbDomain, &snuType);
        // 以上调用 API 会失败，失败原因是内存不足。并把所需要的内存大小传出。
        // 下面是处理非内存不足的错误。
        if (fAPISuccess)
            __leave;
        else if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
        {
            qDebug("LookupAccountName() failed. Error %d/n",GetLastError());
            __leave;
        }

        pUserSID = myheapalloc(cbUserSID);
        if (!pUserSID)
        {
            qDebug("HeapAlloc() failed. Error %d/n", GetLastError());
            __leave;
        }
        szDomain = (TCHAR *) myheapalloc(cbDomain * sizeof(TCHAR));
        if (!szDomain)
        {
            qDebug("HeapAlloc() failed. Error %d/n", GetLastError());
            __leave;
        }
        fAPISuccess = LookupAccountName(NULL, lpszAccountName,
        pUserSID, &cbUserSID, szDomain, &cbDomain, &snuType);
        if (!fAPISuccess)
        {
            qDebug("LookupAccountName() failed. Error %d/n",GetLastError());
            __leave;
        }
        //
        // STEP 2: 取得文件（目录）相关的安全描述符 SD
        //   使用 GetFileSecurity 函数取得一份文件 SD 的拷贝，同样，这个函数也
        // 是被调用两次，第一次同样是取 SD 的内存长度。注意， SD 有两种格式：自相关的
        // （ self-relative ）和 完全的（ absolute ）， GetFileSecurity 只能取到“自
        // 相关的”，而 SetFileSecurity 则需要完全的。这就是为什么需要一个新的 SD ，
        // 而不是直接在 GetFileSecurity 返回的 SD 上进行修改。因为“自相关的”信息
        // 是不完整的。
        fAPISuccess = GetFileSecurity(lpszFileName,
        secInfo, pFileSD, 0, &cbFileSD);
        // 以上调用 API 会失败，失败原因是内存不足。并把所需要的内存大小传出。
        // 下面是处理非内存不足的错误。
        if (fAPISuccess)
        __leave;
        else if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
        {
            qDebug("GetFileSecurity() failed. Error %d/n",GetLastError());
            __leave;
        }
        pFileSD = myheapalloc(cbFileSD);
        if (!pFileSD)
        {
            qDebug("HeapAlloc() failed. Error %d/n", GetLastError());
            __leave;
        }
        fAPISuccess = GetFileSecurity(lpszFileName,
        secInfo, pFileSD, cbFileSD, &cbFileSD);
        if (!fAPISuccess)
        {
            qDebug("GetFileSecurity() failed. Error %d/n",GetLastError());
            __leave;
        }
       //
       // STEP 3: 初始化一个新的 SD
       //
        if (!InitializeSecurityDescriptor(&newSD,SECURITY_DESCRIPTOR_REVISION))
        {
            qDebug("InitializeSecurityDescriptor() failed.Error %d/n", GetLastError());
            __leave;
        }
        //
        // STEP 4: 从 GetFileSecurity  返回的 SD 中取 DACL
        //
        if (!GetSecurityDescriptorDacl(pFileSD, &fDaclPresent, &pACL,&fDaclDefaulted))
        {
            qDebug("GetSecurityDescriptorDacl() failed. Error %d/n",GetLastError());
            __leave;
        }
        //
        // STEP 5: 取  DACL 的内存 size
        //   GetAclInformation可以提供 DACL 的内存大小。只传入一个类型为
        // ACL_SIZE_INFORMATION的 structure 的参数，需 DACL 的信息，是为了
        // 方便我们遍历其中的 ACE 。
        AclInfo.AceCount = 0; // Assume NULL DACL.
        AclInfo.AclBytesFree = 0;
        AclInfo.AclBytesInUse = sizeof(ACL);
        if (pACL == NULL)
            fDaclPresent = FALSE;
        // 如果 DACL 不为空，则取其信息。（大多数情况下“自关联”的 DACL 为空）
        if (fDaclPresent)
        {
            if (!GetAclInformation(pACL, &AclInfo,sizeof(ACL_SIZE_INFORMATION), AclSizeInformation))
            {
                qDebug("GetAclInformation() failed. Error %d/n",GetLastError());
                __leave;
            }
        }
        //
        // STEP 6: 计算新的 ACL 的 size
        //  计算的公式是：原有的 DACL 的 size 加上需要添加的一个 ACE 的 size ，以
        // 及加上一个和 ACE 相关的 SID 的 size ，最后减去两个字节以获得精确的大小。
        cbNewACL = AclInfo.AclBytesInUse + sizeof(ACCESS_ALLOWED_ACE) + GetLengthSid(pUserSID) - sizeof(DWORD);
        //
        // STEP 7: 为新的 ACL 分配内存
        //
        pNewACL = (PACL) myheapalloc(cbNewACL);
        if (!pNewACL)
        {
            qDebug("HeapAlloc() failed. Error %d/n", GetLastError());
            __leave;
        }
        //
        // STEP 8: 初始化新的 ACL 结构
        //
        if (!InitializeAcl(pNewACL, cbNewACL, ACL_REVISION2))
        {
            qDebug("InitializeAcl() failed. Error %d/n",GetLastError());
            __leave;
        }
        //
        // STEP 9 如果文件（目录）  DACL  有数据，拷贝其中的 ACE 到新的 DACL 中
        //
        //   下面的代码假设首先检查指定文件（目录）是否存在的 DACL ，如果有的话，
        // 那么就拷贝所有的 ACE 到新的 DACL 结构中，我们可以看到其遍历的方法是采用
        // ACL_SIZE_INFORMATION结构中的 AceCount 成员来完成的。在这个循环中，
        // 会按照默认的 ACE 的顺序来进行拷贝（ ACE 在 ACL 中的顺序是很关键的），在拷
        // 贝过程中，先拷贝非继承的 ACE （我们知道 ACE 会从上层目录中继承下来）
        //
        newAceIndex = 0;
        if (fDaclPresent && AclInfo.AceCount)
        {
            for (CurrentAceIndex = 0; CurrentAceIndex < AclInfo.AceCount; CurrentAceIndex++)
            {
                //
                // STEP 10: 从 DACL 中取 ACE
                //
                if (!GetAce(pACL, CurrentAceIndex, &pTempAce))
                {
                    qDebug("GetAce() failed. Error %d/n",GetLastError());
                    __leave;
                }
                //
                // STEP 11: 检查是否是非继承的 ACE
                //   如果当前的 ACE 是一个从父目录继承来的 ACE ，那么就退出循环。
                // 因为，继承的 ACE 总是在非继承的 ACE 之后，而我们所要添加的 ACE
                // 应该在已有的非继承的 ACE 之后，所有的继承的 ACE 之前。退出循环
                // 正是为了要添加一个新的 ACE 到新的 DACL 中，这后，我们再把继承的
                // ACE拷贝到新的 DACL 中。
                //
                if (((ACCESS_ALLOWED_ACE *)pTempAce)->Header.AceFlags & INHERITED_ACE)
                    break;
                //
                // STEP 12: 检查要拷贝的 ACE 的 SID 是否和需要加入的 ACE 的 SID 一样，
                // 如果一样，那么就应该废掉已存在的 ACE ，也就是说，同一个用户的存取
                // 权限的设置的 ACE ，在 DACL 中应该唯一。这在里，跳过对同一用户已设置
                // 了的 ACE ，仅是拷贝其它用户的 ACE 。
                //
                if (EqualSid(pUserSID, &(((ACCESS_ALLOWED_ACE *)pTempAce)->SidStart)))
                    continue;
                //
                // STEP 13: 把 ACE 加入到新的 DACL 中
                //  下面的代码中，注意  AddAce  函数的第三个参数，这个参数的意思是
                // ACL中的索引值，意为要把 ACE 加到某索引位置之后，参数 MAXDWORD 的
                // 意思是确保当前的 ACE 是被加入到最后的位置。
                //
                if (!AddAce(pNewACL, ACL_REVISION, MAXDWORD, pTempAce,((PACE_HEADER) pTempAce)->AceSize))
                {
                    qDebug("AddAce() failed. Error %d/n",GetLastError());
                    __leave;
                }
                newAceIndex++;
            }
        }

        //
        // STEP 14: 把一个  access-allowed  的 ACE  加入到新的 DACL 中
        //   前面的循环拷贝了所有的非继承且 SID 为其它用户的 ACE ，退出循环的第一件事
        // 就是加入我们指定的 ACE 。请注意首先先动态装载了一个 AddAccessAllowedAceEx
        // 的 API 函数，如果装载不成功，就调用 AddAccessAllowedAce 函数。前一个函数仅
        // 在 Windows 2000 以后的版本支持， NT 则没有，我们为了使用新版本的函数，我们首
        // 先先检查一下当前系统中可不可以装载这个函数，如果可以则就使用。使用动态链接
        // 比使用静态链接的好处是，程序运行时不会因为没有这个 API 函数而报错。
        //
        // Ex版的函数多出了一个参数 AceFlag （第三人参数），用这个参数我们可以来设置一
        // 个叫 ACE_HEADER 的结构，以便让我们所设置的 ACE 可以被其子目录所继承下去，而
        // AddAccessAllowedAce函数不能定制这个参数，在 AddAccessAllowedAce 函数
        // 中，其会把 ACE_HEADER 这个结构设置成非继承的。
        //
        _AddAccessAllowedAceEx = (AddAccessAllowedAceExFnPtr)
        GetProcAddress(GetModuleHandle(TEXT("advapi32.dll")),"AddAccessAllowedAceEx");
        if (_AddAccessAllowedAceEx)
        {
            if (!_AddAccessAllowedAceEx(pNewACL, ACL_REVISION2, CONTAINER_INHERIT_ACE | OBJECT_INHERIT_ACE , dwAccessMask, pUserSID))
            {
                qDebug("AddAccessAllowedAceEx() failed. Error %d/n",GetLastError());
                __leave;
            }
        }
        else
        {
            if (!AddAccessAllowedAce(pNewACL, ACL_REVISION2,
            dwAccessMask, pUserSID))
            {
                qDebug("AddAccessAllowedAce() failed. Error %d/n",GetLastError());
                __leave;
            }
        }

        //
        // STEP 15: 按照已存在的 ACE 的顺序拷贝从父目录继承而来的 ACE
        //
        if (fDaclPresent && AclInfo.AceCount)
        {
            for (;CurrentAceIndex < AclInfo.AceCount;CurrentAceIndex++)
            {
                //
                // STEP 16: 从文件（目录）的 DACL 中继续取 ACE
                //
                if (!GetAce(pACL, CurrentAceIndex, &pTempAce))
                {
                    qDebug("GetAce() failed. Error %d/n",GetLastError());
                    __leave;
                }
                //
                // STEP 17: 把 ACE 加入到新的 DACL 中
                //
                if (!AddAce(pNewACL, ACL_REVISION, MAXDWORD, pTempAce,((PACE_HEADER) pTempAce)->AceSize))
                {
                    qDebug("AddAce() failed. Error %d/n",GetLastError());
                    __leave;
                }
            }
        }
        //
        // STEP 18: 把新的 ACL 设置到新的 SD 中
        //
        if (!SetSecurityDescriptorDacl(&newSD, TRUE, pNewACL,FALSE))
        {
            qDebug("SetSecurityDescriptorDacl() failed. Error %d/n",GetLastError());
            __leave;
        }
        //
        // STEP 19: 把老的 SD 中的控制标记再拷贝到新的 SD 中，我们使用的是一个叫
        // SetSecurityDescriptorControl() 的 API 函数，这个函数同样只存在于
        // Windows 2000以后的版本中，所以我们还是要动态地把其从 advapi32.dll
        // 中载入，如果系统不支持这个函数，那就不拷贝老的 SD 的控制标记了。
        //
        _SetSecurityDescriptorControl = (SetSecurityDescriptorControlFnPtr)GetProcAddress(GetModuleHandle(TEXT("advapi32.dll")),"SetSecurityDescriptorControl");
        if (_SetSecurityDescriptorControl)
        {
            SECURITY_DESCRIPTOR_CONTROL controlBitsOfInterest = 0;
            SECURITY_DESCRIPTOR_CONTROL controlBitsToSet = 0;
            SECURITY_DESCRIPTOR_CONTROL oldControlBits = 0;
            DWORD dwRevision = 0;
            if (!GetSecurityDescriptorControl(pFileSD, &oldControlBits, &dwRevision))
            {
                qDebug("GetSecurityDescriptorControl() failed.Error %d/n", GetLastError());
                __leave;
            }
            if (oldControlBits & SE_DACL_AUTO_INHERITED)
            {
                controlBitsOfInterest = SE_DACL_AUTO_INHERIT_REQ | SE_DACL_AUTO_INHERITED ;
                controlBitsToSet = controlBitsOfInterest;
            }
            else if (oldControlBits & SE_DACL_PROTECTED)
            {
                controlBitsOfInterest = SE_DACL_PROTECTED;
                controlBitsToSet = controlBitsOfInterest;
            }
            if (controlBitsOfInterest)
            {
                if (!_SetSecurityDescriptorControl(&newSD,controlBitsOfInterest,controlBitsToSet))
                {
                    qDebug("SetSecurityDescriptorControl() failed.Error %d/n", GetLastError());
                    __leave;
                }
            }
        }
        //
        // STEP 20: 把新的 SD 设置设置到文件的安全属性中（千山万水啊，终于到了）
        //
        if (!SetFileSecurity(lpszFileName, secInfo,&newSD))
        {
            qDebug("SetFileSecurity() failed. Error %d/n",GetLastError());
            __leave;
        }
        fResult = TRUE;
    }
    __finally
    {
        //
        // STEP 21: 释放已分配的内存，以免 Memory Leak
        //
        if (pUserSID) myheapfree(pUserSID);
        if (szDomain) myheapfree(szDomain);
        if (pFileSD) myheapfree(pFileSD);
        if (pNewACL) myheapfree(pNewACL);
    }

    return fResult;
}
