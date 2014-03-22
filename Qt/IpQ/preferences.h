#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QString>
#include <QSettings>
#include <QStringList>
#include <QMutex>
#include <QFile>

// 偏好设置类
class Preferences
{
public:
    Preferences();
    ~Preferences();

    void reset();
    void load();
    void save();
    void openLogFile();

    QString themeFile;          // 主题
    QString langFile;            // 语言
    QString userName;           // 用户名
    QString groupName;          // 用户组
    QString groupNameHistory;   // 历史组
    QStringList groupNameList;  // 组名表
//    bool isSealed;              // 密封
    bool isReadCheck;           // 只读检查
    bool isNoSoundAlarm;        // 声音警告
    bool isNoAutoPopupMsg;      // 自动弹出
//    bool isQuoteMsg;            // 引用消息
    bool noAutoCloseMsgWindowWhenReply; // 回复自动关闭

//    bool isSingleClickSendWindow;//单击发送

//    QString quoteMark;          // 引用标示
    QString noticeSound;        // 通知声音

    bool isLogMsg;              // 记录日志
 //   bool noLogLockMsgBeforeOpen;// 记录锁定的消息在打开前
    bool isLogLoginName;        // 记录登录名
    bool isLogIP;               // 记录IP
    QString logFilePath;        // 记录文件路径
    QFile logFile;              // 记录文件

    QString lastSaveFilePath;   // 最后保存文件路径
    QString lastSendFilePath;   // 最后发送文件路径
    QString lastSendDirPath;    // 最后发送路径

    QStringList userSearchList; // 用户查寻列表
    QString userSearchHistory;  // 用户历史查寻
    bool isSearchAllColumns;    // 查找所有

//    QString transferCodecName;  // 传输编码

    //手动指定广播地址列表
//    QStringList userSpecifiedBroadcastIpList;
//    QString userSpecifiedBroadcastIp;
};

#endif // PREFERENCES_H
