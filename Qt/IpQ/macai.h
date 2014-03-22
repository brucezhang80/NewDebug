#ifndef MACAI_H
#define MACAI_H

#include <QString>
#include <QMap>
#include <QPoint>
#include <QColor>

class QSettings;
class QIcon;
class QTranslator;

class MainWindow;
class AboutDialog;
class MessageThread;
class SystemTray;
class UserManager;
class Preferences;
class UserTreeWidget;
class WindowManager;
class MessageBox;
class TransferFileWindow;
class SendFileTreeWidget;
class RecvFileTreeWidget;
class FileTcpServer;
class SendUsersTreeWidget;
//class SendHistoryTreeWidget;
class SendHistoryTableView;
class SendFilePropertise;
class SystemSettings;
class ConfigurationPage;
class MessageConfigPage;
class FolderConfigPage;
class HelperConfigPage;

namespace Macai
{
    extern QTranslator          *translator;
    extern MainWindow           *mainWindow;
    extern AboutDialog          *aboutDialog;
    extern QSettings            *settings;
    extern UserManager          *userManager;
    extern MessageThread        *messageThread;
    extern SystemTray           *systemTray;
    extern QMap<QString, QIcon*> iconSet;
    extern Preferences          *preferences;
    extern UserTreeWidget       *userTreeWidget;
    extern WindowManager        *windowManager;
    extern MessageBox           *messageBox;
    extern TransferFileWindow   *transferFileWindow;
    extern SendFileTreeWidget   *sendFileTreeWidget;
    extern RecvFileTreeWidget   *recvFileTreeWidget;
    extern SendUsersTreeWidget  *sendUsersTreeWidget;
//    extern SendHistoryTreeWidget*sendHistoryTreeWidget;
    extern SendHistoryTableView *sendHistoryTableView;
    extern FileTcpServer        *fileTcpServer;
    extern SendFilePropertise   *sendFilePropertise;
    extern SystemSettings       *systemSettings;
    extern ConfigurationPage    *configurationPage;
    extern MessageConfigPage    *messageConfigPage;
    extern FolderConfigPage     *folderConfigPage;
    extern HelperConfigPage     *helperConfigPage;

    void MacaiStart(QString path);
    void MacaiEnd();

    void setThemeAll(const QString &themeFile);
    void setThemeBkColor(const QColor &color);

    void retranslateAllUi(const QString &langFile);

    QString fileCountString(int i);
    QPoint randomNearMiddlePoint();
}

#endif // MACAI_H
