#ifndef NEWPROJECTDLG_H
#define NEWPROJECTDLG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>

class NewProjectDlg : public QDialog
{
    Q_OBJECT
public:
    explicit NewProjectDlg(QWidget *parent = 0);
    
signals:
    
public slots:
    
public:
    QLineEdit       *m_proUrlLedit;
    QLineEdit       *m_proTitleLedit;
    QLineEdit       *m_proSavepathLedit;
    QPushButton     *m_proSetSavepathPbtn;
    QPushButton     *m_proAddPbtn;

};

#endif // NEWPROJECTDLG_H
