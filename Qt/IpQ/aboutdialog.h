#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

class QWidget;
class QDialogButtonBox;
class QLabel;

class AboutDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AboutDialog(QWidget *parent = 0);
    
    // 为了方便设置多国语言动态切换
    void retranslateUi();

signals:
    
public slots:
    
private:
    QLabel              *m_lbInfo;
    QDialogButtonBox    *m_buttonBox;
};

#endif // ABOUTDIALOG_H
