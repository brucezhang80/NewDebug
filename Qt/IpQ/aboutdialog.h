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
    
    // Ϊ�˷������ö�����Զ�̬�л�
    void retranslateUi();

signals:
    
public slots:
    
private:
    QLabel              *m_lbInfo;
    QDialogButtonBox    *m_buttonBox;
};

#endif // ABOUTDIALOG_H
