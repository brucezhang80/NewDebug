#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QString>
#include <QSettings>
#include <QStringList>
#include <QMutex>
#include <QFile>

// ƫ��������
class Preferences
{
public:
    Preferences();
    ~Preferences();

    void reset();
    void load();
    void save();
    void openLogFile();

    QString themeFile;          // ����
    QString langFile;            // ����
    QString userName;           // �û���
    QString groupName;          // �û���
    QString groupNameHistory;   // ��ʷ��
    QStringList groupNameList;  // ������
//    bool isSealed;              // �ܷ�
    bool isReadCheck;           // ֻ�����
    bool isNoSoundAlarm;        // ��������
    bool isNoAutoPopupMsg;      // �Զ�����
//    bool isQuoteMsg;            // ������Ϣ
    bool noAutoCloseMsgWindowWhenReply; // �ظ��Զ��ر�

//    bool isSingleClickSendWindow;//��������

//    QString quoteMark;          // ���ñ�ʾ
    QString noticeSound;        // ֪ͨ����

    bool isLogMsg;              // ��¼��־
 //   bool noLogLockMsgBeforeOpen;// ��¼��������Ϣ�ڴ�ǰ
    bool isLogLoginName;        // ��¼��¼��
    bool isLogIP;               // ��¼IP
    QString logFilePath;        // ��¼�ļ�·��
    QFile logFile;              // ��¼�ļ�

    QString lastSaveFilePath;   // ��󱣴��ļ�·��
    QString lastSendFilePath;   // ������ļ�·��
    QString lastSendDirPath;    // �����·��

    QStringList userSearchList; // �û���Ѱ�б�
    QString userSearchHistory;  // �û���ʷ��Ѱ
    bool isSearchAllColumns;    // ��������

//    QString transferCodecName;  // �������

    //�ֶ�ָ���㲥��ַ�б�
//    QStringList userSpecifiedBroadcastIpList;
//    QString userSpecifiedBroadcastIp;
};

#endif // PREFERENCES_H
