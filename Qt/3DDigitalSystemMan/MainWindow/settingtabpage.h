#ifndef SETTINGTABPAGE_H
#define SETTINGTABPAGE_H

#include <QWidget>
#include <QToolButton>
#include <QHBoxLayout>
#include <QSignalMapper>

const int NUM_BTN = 6;
class SettingTabPage : public QWidget
{
    Q_OBJECT
public:
    explicit SettingTabPage(QWidget *parent = 0);
    
    void setButtonMapping();

signals:
    
public slots:
    void clickedButton(QWidget*);

public:
    QToolButton *m_modelSettingBtn;
    QToolButton *m_scriptSettingBtn;
    QToolButton *m_dataSettingBtn;

    QToolButton *m_systemSettingBtn;
    QToolButton *m_updateSysBtn;
    QToolButton *m_aboutSysBtn;

private:
    QSignalMapper         *m_signalMapper;
};

#endif // SETTINGTABPAGE_H
