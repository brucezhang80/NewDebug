#ifndef SYSTEMTRAYICON_H
#define SYSTEMTRAYICON_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>

class SystemTrayIcon : public QWidget
{
    Q_OBJECT
public:
    explicit SystemTrayIcon(QIcon icon, QWidget *parent = 0);
    void setIcon(const QIcon);
    void setToolTips(const QString &);
    void showTrayIconMessage(const QString &title, const QString &message,
                             QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::Information, int millisecondsTimeoutHint = 10000 );
    QRect geometry() const;

private:
    void createActions();
    void createTrayIcon();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

signals:
    
private slots:
    void show();
private:
    QAction         *m_hideAction;
    QAction         *m_showAction;
    QAction         *m_maximizeAction;
    QAction         *m_restoreAction;
    QAction         *m_quitAction;

    QSystemTrayIcon *m_trayIcon;
    QMenu           *m_trayIconMenu;

    QWidget         *m_thisParent;
    
};

#endif // SYSTEMTRAYICON_H
