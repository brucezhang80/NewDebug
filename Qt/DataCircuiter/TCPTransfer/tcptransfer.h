#ifndef TCPTRANSFER_H
#define TCPTRANSFER_H

#include <QWidget>

class FriendsView;
class FilesView;
class HistoryUpload;

namespace Ui {
class TCPTransfer;
}

class TCPTransfer : public QWidget
{
    Q_OBJECT
    
public:
    explicit TCPTransfer(QWidget *parent = 0);
    ~TCPTransfer();

    QStringList getProjectPathList() const;

signals:
    void currentStackWidgetSingal(int);
    void currentProjectPath(const QString &path);

private slots:
    void setCurrentStackWidgetSlot();
    void updateControl(bool bEnabled);
    void currentComboboxIndex(int);

protected:
    void updateButton(int index);
private:
    Ui::TCPTransfer *ui;
    FriendsView     *m_friendsView;
    FilesView       *m_filesView;
    HistoryUpload   *m_historyUpload;

    bool        bUploading;

    QString serverName;
    QString serverPubRoot;
};

#endif // TCPTRANSFER_H
