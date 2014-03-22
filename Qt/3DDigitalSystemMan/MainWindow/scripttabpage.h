#ifndef SCRIPTTABPAGE_H
#define SCRIPTTABPAGE_H

#include <QWidget>
#include <QToolButton>
#include <QHBoxLayout>

class ScriptTabPage : public QWidget
{
    Q_OBJECT
public:
    explicit ScriptTabPage(QWidget *parent = 0);
    void stateMachineWidget();

signals:
    
public slots:
    void OnAddScriptBtnSlot();
    void OnRunScriptBtnSlot();
    void OnEditScriptBtnSlot();
    void OnUpdateBtnSlot();

public:
    QToolButton *m_addscriptBtn;
    QToolButton *m_runscriptBtn;
    QToolButton *m_editscriptBtn;
};
#endif // SCRIPTTABPAGE_H
