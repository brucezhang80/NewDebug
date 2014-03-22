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
    //ֻ��Ҫipv4��IP��ַ
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

    // �����ǲ��ᱻִ�е�
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

    return (res == 0 || res == 2118); // 2118��ʾ�Ѿ����ڵĹ���
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


    // argv[1] �C �ļ���Ŀ¼����
    // argv[2] �C �û����飩��
    // GENERIC_ALL��ʾ���е�Ȩ�ޣ�����һϵ�е� NTFS Ȩ�޵Ļ�
    //   NTFS���ļ�Ȩ�޺�ϸ������ο� MSDN ��
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

    return (res == 0 || res == 2118); // 2118��ʾ�Ѿ����ڵĹ���
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

//ʹ�� Windows �� HeapAlloc �������ж�̬�ڴ����
#define myheapalloc(x) (HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, x))
#define myheapfree(x) (HeapFree(GetProcessHeap(), 0, x))
typedef BOOL (WINAPI *SetSecurityDescriptorControlFnPtr)(
  IN PSECURITY_DESCRIPTOR pSecurityDescriptor,
  IN SECURITY_DESCRIPTOR_CONTROL ControlBitsOfInterest,
  IN SECURITY_DESCRIPTOR_CONTROL ControlBitsToSet);
typedef BOOL (WINAPI *AddAccessAllowedAceExFnPtr)(PACL pAcl,DWORD dwAceRevision,DWORD AceFlags,DWORD AccessMask,PSID pSid);

// ���ӹ����ļ��еķ���Ȩ��
BOOL AddAccessRights(TCHAR *lpszFileName, TCHAR *lpszAccountName,DWORD dwAccessMask)
{
  // ���� SID ����
  SID_NAME_USE  snuType;
  // ������ LookupAccountName ��صı�����ע�⣬ȫΪ 0 ��Ҫ�ڳ����ж�̬���䣩
  TCHAR *    szDomain    = NULL;
  DWORD     cbDomain    = 0;
  LPVOID     pUserSID    = NULL;
  DWORD     cbUserSID   = 0;
  // ���ļ���صİ�ȫ������  SD  �ı���
  PSECURITY_DESCRIPTOR pFileSD = NULL;   // �ṹ����
  DWORD     cbFileSD    = 0;    // SD�� size
  // һ���µ� SD �ı��������ڹ����µ� ACL �������е� ACL ����Ҫ�¼ӵ� ACL ����������
  SECURITY_DESCRIPTOR newSD;
  // �� ACL  ��صı���
  PACL      pACL      = NULL;
  BOOL      fDaclPresent;
  BOOL      fDaclDefaulted;
  ACL_SIZE_INFORMATION AclInfo;
  // һ���µ�  ACL  ����
  PACL      pNewACL    = NULL; //�ṹָ�����
  DWORD     cbNewACL    = 0;   //ACL�� size
  // һ����ʱʹ�õ�  ACE  ����
  LPVOID     pTempAce    = NULL;
  UINT      CurrentAceIndex = 0; //ACE�� ACL �е�λ��
  UINT      newAceIndex = 0; //����� ACE �� ACL �е�λ��
  //API�����ķ���ֵ���������еĺ���������ʧ�ܡ�
  BOOL      fResult;
  BOOL      fAPISuccess;
  SECURITY_INFORMATION secInfo = DACL_SECURITY_INFORMATION;
  // ����������������µ� API ���������� Windows 2000 ���ϰ汾�Ĳ���ϵͳ֧�֡�
  // �ڴ˽��� Advapi32.dll �ļ��ж�̬���롣�����ʹ�� VC++ 6.0 ������򣬶�������
  // ʹ�������������ľ�̬���ӡ�����Ϊ��ı�����ϣ� /D_WIN32_WINNT=0x0500
  // �ı������������ȷ����� SDK ��ͷ�ļ��� lib �ļ������µġ�
  SetSecurityDescriptorControlFnPtr _SetSecurityDescriptorControl = NULL;
  AddAccessAllowedAceExFnPtr _AddAccessAllowedAceEx = NULL;
  __try
    {
        //
        // STEP 1: ͨ���û���ȡ�� SID
        // ����һ���� LookupAccountName ���������������Σ���һ����ȡ������Ҫ
        // ���ڴ�Ĵ�С��Ȼ�󣬽����ڴ���䡣�ڶ��ε��ò���ȡ�����û����ʻ���Ϣ��
        // LookupAccountNameͬ������ȡ�����û������û������Ϣ������ο� MSDN ��
        //
        fAPISuccess = LookupAccountName(NULL, lpszAccountName,
        pUserSID, &cbUserSID, szDomain, &cbDomain, &snuType);
        // ���ϵ��� API ��ʧ�ܣ�ʧ��ԭ�����ڴ治�㡣��������Ҫ���ڴ��С������
        // �����Ǵ�����ڴ治��Ĵ���
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
        // STEP 2: ȡ���ļ���Ŀ¼����صİ�ȫ������ SD
        //   ʹ�� GetFileSecurity ����ȡ��һ���ļ� SD �Ŀ�����ͬ�����������Ҳ
        // �Ǳ��������Σ���һ��ͬ����ȡ SD ���ڴ泤�ȡ�ע�⣬ SD �����ָ�ʽ������ص�
        // �� self-relative ���� ��ȫ�ģ� absolute ���� GetFileSecurity ֻ��ȡ������
        // ��صġ����� SetFileSecurity ����Ҫ��ȫ�ġ������Ϊʲô��Ҫһ���µ� SD ��
        // ������ֱ���� GetFileSecurity ���ص� SD �Ͻ����޸ġ���Ϊ������صġ���Ϣ
        // �ǲ������ġ�
        fAPISuccess = GetFileSecurity(lpszFileName,
        secInfo, pFileSD, 0, &cbFileSD);
        // ���ϵ��� API ��ʧ�ܣ�ʧ��ԭ�����ڴ治�㡣��������Ҫ���ڴ��С������
        // �����Ǵ�����ڴ治��Ĵ���
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
       // STEP 3: ��ʼ��һ���µ� SD
       //
        if (!InitializeSecurityDescriptor(&newSD,SECURITY_DESCRIPTOR_REVISION))
        {
            qDebug("InitializeSecurityDescriptor() failed.Error %d/n", GetLastError());
            __leave;
        }
        //
        // STEP 4: �� GetFileSecurity  ���ص� SD ��ȡ DACL
        //
        if (!GetSecurityDescriptorDacl(pFileSD, &fDaclPresent, &pACL,&fDaclDefaulted))
        {
            qDebug("GetSecurityDescriptorDacl() failed. Error %d/n",GetLastError());
            __leave;
        }
        //
        // STEP 5: ȡ  DACL ���ڴ� size
        //   GetAclInformation�����ṩ DACL ���ڴ��С��ֻ����һ������Ϊ
        // ACL_SIZE_INFORMATION�� structure �Ĳ������� DACL ����Ϣ����Ϊ��
        // �������Ǳ������е� ACE ��
        AclInfo.AceCount = 0; // Assume NULL DACL.
        AclInfo.AclBytesFree = 0;
        AclInfo.AclBytesInUse = sizeof(ACL);
        if (pACL == NULL)
            fDaclPresent = FALSE;
        // ��� DACL ��Ϊ�գ���ȡ����Ϣ�������������¡��Թ������� DACL Ϊ�գ�
        if (fDaclPresent)
        {
            if (!GetAclInformation(pACL, &AclInfo,sizeof(ACL_SIZE_INFORMATION), AclSizeInformation))
            {
                qDebug("GetAclInformation() failed. Error %d/n",GetLastError());
                __leave;
            }
        }
        //
        // STEP 6: �����µ� ACL �� size
        //  ����Ĺ�ʽ�ǣ�ԭ�е� DACL �� size ������Ҫ��ӵ�һ�� ACE �� size ����
        // ������һ���� ACE ��ص� SID �� size ������ȥ�����ֽ��Ի�þ�ȷ�Ĵ�С��
        cbNewACL = AclInfo.AclBytesInUse + sizeof(ACCESS_ALLOWED_ACE) + GetLengthSid(pUserSID) - sizeof(DWORD);
        //
        // STEP 7: Ϊ�µ� ACL �����ڴ�
        //
        pNewACL = (PACL) myheapalloc(cbNewACL);
        if (!pNewACL)
        {
            qDebug("HeapAlloc() failed. Error %d/n", GetLastError());
            __leave;
        }
        //
        // STEP 8: ��ʼ���µ� ACL �ṹ
        //
        if (!InitializeAcl(pNewACL, cbNewACL, ACL_REVISION2))
        {
            qDebug("InitializeAcl() failed. Error %d/n",GetLastError());
            __leave;
        }
        //
        // STEP 9 ����ļ���Ŀ¼��  DACL  �����ݣ��������е� ACE ���µ� DACL ��
        //
        //   ����Ĵ���������ȼ��ָ���ļ���Ŀ¼���Ƿ���ڵ� DACL ������еĻ���
        // ��ô�Ϳ������е� ACE ���µ� DACL �ṹ�У����ǿ��Կ���������ķ����ǲ���
        // ACL_SIZE_INFORMATION�ṹ�е� AceCount ��Ա����ɵġ������ѭ���У�
        // �ᰴ��Ĭ�ϵ� ACE ��˳�������п����� ACE �� ACL �е�˳���Ǻܹؼ��ģ����ڿ�
        // �������У��ȿ����Ǽ̳е� ACE ������֪�� ACE ����ϲ�Ŀ¼�м̳�������
        //
        newAceIndex = 0;
        if (fDaclPresent && AclInfo.AceCount)
        {
            for (CurrentAceIndex = 0; CurrentAceIndex < AclInfo.AceCount; CurrentAceIndex++)
            {
                //
                // STEP 10: �� DACL ��ȡ ACE
                //
                if (!GetAce(pACL, CurrentAceIndex, &pTempAce))
                {
                    qDebug("GetAce() failed. Error %d/n",GetLastError());
                    __leave;
                }
                //
                // STEP 11: ����Ƿ��ǷǼ̳е� ACE
                //   �����ǰ�� ACE ��һ���Ӹ�Ŀ¼�̳����� ACE ����ô���˳�ѭ����
                // ��Ϊ���̳е� ACE �����ڷǼ̳е� ACE ֮�󣬶�������Ҫ��ӵ� ACE
                // Ӧ�������еķǼ̳е� ACE ֮�����еļ̳е� ACE ֮ǰ���˳�ѭ��
                // ����Ϊ��Ҫ���һ���µ� ACE ���µ� DACL �У���������ٰѼ̳е�
                // ACE�������µ� DACL �С�
                //
                if (((ACCESS_ALLOWED_ACE *)pTempAce)->Header.AceFlags & INHERITED_ACE)
                    break;
                //
                // STEP 12: ���Ҫ������ ACE �� SID �Ƿ����Ҫ����� ACE �� SID һ����
                // ���һ������ô��Ӧ�÷ϵ��Ѵ��ڵ� ACE ��Ҳ����˵��ͬһ���û��Ĵ�ȡ
                // Ȩ�޵����õ� ACE ���� DACL ��Ӧ��Ψһ�������������ͬһ�û�������
                // �˵� ACE �����ǿ��������û��� ACE ��
                //
                if (EqualSid(pUserSID, &(((ACCESS_ALLOWED_ACE *)pTempAce)->SidStart)))
                    continue;
                //
                // STEP 13: �� ACE ���뵽�µ� DACL ��
                //  ����Ĵ����У�ע��  AddAce  �����ĵ����������������������˼��
                // ACL�е�����ֵ����ΪҪ�� ACE �ӵ�ĳ����λ��֮�󣬲��� MAXDWORD ��
                // ��˼��ȷ����ǰ�� ACE �Ǳ����뵽����λ�á�
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
        // STEP 14: ��һ��  access-allowed  �� ACE  ���뵽�µ� DACL ��
        //   ǰ���ѭ�����������еķǼ̳��� SID Ϊ�����û��� ACE ���˳�ѭ���ĵ�һ����
        // ���Ǽ�������ָ���� ACE ����ע�������ȶ�̬װ����һ�� AddAccessAllowedAceEx
        // �� API ���������װ�ز��ɹ����͵��� AddAccessAllowedAce ������ǰһ��������
        // �� Windows 2000 �Ժ�İ汾֧�֣� NT ��û�У�����Ϊ��ʹ���°汾�ĺ�����������
        // ���ȼ��һ�µ�ǰϵͳ�пɲ�����װ���������������������ʹ�á�ʹ�ö�̬����
        // ��ʹ�þ�̬���ӵĺô��ǣ���������ʱ������Ϊû����� API ����������
        //
        // Ex��ĺ��������һ������ AceFlag �������˲�������������������ǿ���������һ
        // ���� ACE_HEADER �Ľṹ���Ա������������õ� ACE ���Ա�����Ŀ¼���̳���ȥ����
        // AddAccessAllowedAce�������ܶ�������������� AddAccessAllowedAce ����
        // �У����� ACE_HEADER ����ṹ���óɷǼ̳еġ�
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
        // STEP 15: �����Ѵ��ڵ� ACE ��˳�򿽱��Ӹ�Ŀ¼�̳ж����� ACE
        //
        if (fDaclPresent && AclInfo.AceCount)
        {
            for (;CurrentAceIndex < AclInfo.AceCount;CurrentAceIndex++)
            {
                //
                // STEP 16: ���ļ���Ŀ¼���� DACL �м���ȡ ACE
                //
                if (!GetAce(pACL, CurrentAceIndex, &pTempAce))
                {
                    qDebug("GetAce() failed. Error %d/n",GetLastError());
                    __leave;
                }
                //
                // STEP 17: �� ACE ���뵽�µ� DACL ��
                //
                if (!AddAce(pNewACL, ACL_REVISION, MAXDWORD, pTempAce,((PACE_HEADER) pTempAce)->AceSize))
                {
                    qDebug("AddAce() failed. Error %d/n",GetLastError());
                    __leave;
                }
            }
        }
        //
        // STEP 18: ���µ� ACL ���õ��µ� SD ��
        //
        if (!SetSecurityDescriptorDacl(&newSD, TRUE, pNewACL,FALSE))
        {
            qDebug("SetSecurityDescriptorDacl() failed. Error %d/n",GetLastError());
            __leave;
        }
        //
        // STEP 19: ���ϵ� SD �еĿ��Ʊ���ٿ������µ� SD �У�����ʹ�õ���һ����
        // SetSecurityDescriptorControl() �� API �������������ͬ��ֻ������
        // Windows 2000�Ժ�İ汾�У��������ǻ���Ҫ��̬�ذ���� advapi32.dll
        // �����룬���ϵͳ��֧������������ǾͲ������ϵ� SD �Ŀ��Ʊ���ˡ�
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
        // STEP 20: ���µ� SD �������õ��ļ��İ�ȫ�����У�ǧɽ��ˮ�������ڵ��ˣ�
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
        // STEP 21: �ͷ��ѷ�����ڴ棬���� Memory Leak
        //
        if (pUserSID) myheapfree(pUserSID);
        if (szDomain) myheapfree(szDomain);
        if (pFileSD) myheapfree(pFileSD);
        if (pNewACL) myheapfree(pNewACL);
    }

    return fResult;
}
