#ifndef ADDRECIEVERDLG_H
#define ADDRECIEVERDLG_H
#include "ui_AddRecieverDlg.h"
#include <QDialog>
#include <QMap>

class AddRecieverDlg : public QDialog
{
    Q_OBJECT
public:
    explicit AddRecieverDlg(QWidget *parent = 0);
    
    void retranslateUi();

signals:
    
private slots:
    void add();
    void del();
    void ok();

private:
    void initialize();

    QMap<QString, QString>      m_userInfoMap;
    Ui::AddRecieverDlg *ui;
};

#endif // ADDRECIEVERDLG_H
