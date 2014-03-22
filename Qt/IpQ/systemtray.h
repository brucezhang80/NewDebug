#ifndef SYSTEMTRAY_H
#define SYSTEMTRAY_H

#include <QObject>
#include "main_window.h"

class QEvent;
class QMenu;
class QSystemTrayIcon;
class QAction;
class QMouseEvent;

class SystemTray : public QObject
{
    Q_OBJECT
public:
    explicit SystemTray(QObject *parent = 0);
    ~SystemTray();
    void retranslateUi();
    void create();

    QRect geometry();
    void showTooltip(bool bShow = true);

private:
    void createActions();
    void createTrayIcon();
    void createMainWindow();
    void destroyMainWindow();
    void visibleMainWindow();

private slots:
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void about();
    void show();
    void hide();
    void quit();
    void transfer();
    void config();


private:
    QAction         *m_configAction;
    QAction         *m_transferAction;
    QAction         *m_hideAction;
    QAction         *m_showAction;
    QAction         *m_aboutAction;
    QAction         *m_quitAction;

    QSystemTrayIcon  *m_trayIcon;
    QMenu           *m_trayIconMenu;
};

#endif // SYSTEMTRAY_H
