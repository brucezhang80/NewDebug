#ifndef IPQ_H
#define IPQ_H
#include <QChar>
#include <QString>
/**************************************************************************
**************************************************************************
*/
#define MODE                        0x000000ffUL
#define OPTION                      0xffffff00UL

/* header */
#define IPQ_VERSION                 0x0001
#define IPQ_DEFAULTPORT             0x45000

/* command */
#define IPQ_NO_OPERATION           0x00000000UL
#define IPQ_BROADCAST_ENTRY        0x00000001UL
#define IPQ_BROADCAST_EXIT         0x00000002UL
#define IPQ_BROADCAST_ANSENTRY     0x00000003UL
#define IPQ_BROADCAST_ABSENCE      0x00000004UL

#define  IpQBrIsGetList            0x00000010UL
#define  IpQOkGetList              0x00000011UL
#define  IpQGetList                0x00000012UL
#define  IpQAnsList                0x00000013UL
#define  IpQBrIsGetList2           0x00000018UL

#define  IPQ_SEND_MESSAGE          0x00000020UL
#define  IPQ_RECV_MESSAGE          0x00000021UL
#define  IPQ_READ_MESSAGE          0x00000030UL
#define  IPQ_DEL_MESSAGE           0x00000031UL
#define  IPQ_ANSREAD_MESSAGE       0x00000032UL
#define  IPQ_SEND_FILE             0x00000040UL
#define  IPQ_RECV_FILE             0x00000041UL

#define  IPQ_GET_INFO              0x00000011UL
#define  IPQ_SEND_INFO             0x00000011UL

#define  IPQ_GET_FILE_DATA         0x00000060UL
#define  IPQ_RELEASE_FILES         0x00000061UL
#define  IPQ_GET_DIR_FILES         0x00000062UL

#define  IPQ_GET_PUBKEY            0x00000072UL
#define  IPQ_ANS_PUBKEY            0x00000073UL

/* file types for fileattach command */
#define  IPQ_FILE_REGULAR          0x00000001UL
#define  IPQ_FILE_DIR              0x00000002UL
#define  IPQ_FILE_PARENTDIR        0x00000003UL	// return parent directory
#define  IPQ_FILE_SYSLINK          0x00000004UL


/* extend attribute types for fileattach command */
#define IPQ_FILE_UID                0x00000001UL
#define IPQ_FILE_USERNAME           0x00000002UL	// uid by string
#define IPQ_FILE_GID                0x00000003UL
#define IPQ_FILE_GROUPNAME          0x00000004UL	// gid by string
#define IPQ_FILE_PERM               0x00000010UL	// for UNIX
#define IPQ_FILE_MAJORNO            0x00000011UL	// for UNIX devfile
#define IPQ_FILE_MINORNO            0x00000012UL	// for UNIX devfile
#define IPQ_FILE_CTIME              0x00000013UL	// for UNIX
#define IPQ_FILE_MTIME              0x00000014UL
#define IPQ_FILE_ATIME              0x00000015UL
#define IPQ_FILE_CREATETIME         0x00000016UL
#define IPQ_FILE_CREATOR            0x00000020UL	// for Mac
#define IPQ_FILE_FILETYPE           0x00000021UL	// for Mac
#define IPQ_FILE_FINDERINFO         0x00000022UL	// for Mac
#define IPQ_FILE_ACL                0x00000030UL
#define IPQ_FILE_ALIASFNAME         0x00000040UL	// alias fname
#define IPQ_FILE_UNICODEFNAME       0x00000041UL	// UNICODE fname

#define FILELIST_SEPARATOR	'\a'
#define HOSTLIST_SEPARATOR	'\a'
#define HOSTLIST_DUMMY		"\b"

// Response file packet
#define TRANSFER_FILE_HEADER_SIZE_LENGTH      4
#define TRANSFER_FILE_FILE_SIZE_LENGTH        8

#endif // IPQ_H
