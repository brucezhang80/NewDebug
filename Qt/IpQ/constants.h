#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "ipq.h"

static const QString Driver = "D:";
static const QString Company = "SouthGis";
static const QString Program = "IPQ";
static const QString AppNamePre = "IPQ_";
static const QString DefualtTheme = ":/themes/default.qss";

static const QChar CommandSeperator = ':';
static const QChar ExtendInfoSeperator = '\0';
static const QString FileNameEscape = "\a\a";
static const QString FileNameBefore = "::";
static const QString FileNameAfter = ":";

static const int MsgVersionPos = 0;
static const int MsgPacketNoPos = 1;
static const int MsgLogNamePos = 2;
static const int MsgHostPos = 3;
static const int MsgFlagsPos = 4;
static const int MsgAdditionInfoPos = 5;
static const int MsgExtendedInfoPos = 6;

static const int MsgNormalFieldCount = 6;
static const int SendMsgProcessInterval = 2000;     // 2秒发送一次广播
static const int MaxResendTimes = 8;

static const qint64 ONE_KB = 1024.0;
static const qint64 ONE_MB = 1024*1024.0;

static const QString IPQSendPropName = "sendprop";

static const QString SqlDatabaseName = "iqp.data.db";
static const QString SqlNameFileHistory = "FileHistory";
static const QString SqlNameFileRecieved = "SqlNameFileRecved";
static const QString SqlTableFileHistory = "file_history_table";
static const QString SqlTableFileRecieved = "file_recieved_table";

#define RECV_FILE_ID_POS            0
#define RECV_FILE_NAME_POS          1
#define RECV_FILE_SIZE_POS          2
#define RECV_FILE_MTIME_POS         3
#define RECV_FILE_ATTR_POS          4
#define RECV_FILE_EXTEND_ATTR_POS   5

#define USER_VIEW_NAME_COLUMN              0
#define USER_VIEW_GROUP_COLUMN             1
#define USER_VIEW_HOST_COLUMN              2
#define USER_VIEW_IP_COLUMN                3
#define USER_VIEW_LOGIN_NAME_COLUMN        4
#define USER_VIEW_DISPLAY_LEVEL_COLUMN     5

#define TRANSFER_FILE_VIEW_NO_COLUMN       0
#define TRANSFER_FILE_VIEW_FILE_COLUMN     1
#define TRANSFER_FILE_VIEW_SIZE_COLUMN     2
#define TRANSFER_FILE_VIEW_STATS_COLUMN    3
#define TRANSFER_FILE_VIEW_USER_COLUMN     4
#define TRANSFER_FILE_VIEW_KEY_COLUMN      10

#define TRANSFER_FILE_HEADER_SIZE_LENGTH      4
#define TRANSFER_FILE_FILE_SIZE_LENGTH        8
#endif // CONSTANTS_H
