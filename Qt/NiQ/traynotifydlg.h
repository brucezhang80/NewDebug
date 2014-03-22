#ifndef TRAYNOTIFYDLG_H
#define TRAYNOTIFYDLG_H

#include <QDialog>
#include <QMouseEvent>
#include <QtEvents>

namespace Ui {
class TrayNotifyDlg;
}

class TrayNotifyDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit TrayNotifyDlg(QWidget *parent = 0);
    ~TrayNotifyDlg();
    void setNotifyMessage(const QString&);


    enum TaskbarPos
    {
        Left,
        Right,
        Top,
        Bottom
    };
    TrayNotifyDlg::TaskbarPos taskbarDirect();
    TrayNotifyDlg::TaskbarPos taskbarPos;
    QString     nameFrom;
    void moveTo(const int &xbase, const int &ybase, TrayNotifyDlg::TaskbarPos taskbarPos = Bottom);

signals:
    void showMessage(const QString&);

protected:
    void mousePressEvent(QMouseEvent *);
    bool eventFilter(QObject *obj, QEvent *event);
private:
    Ui::TrayNotifyDlg *ui;
    QTimer      *m_timeClose;
};

#endif // TRAYNOTIFYDLG_H
