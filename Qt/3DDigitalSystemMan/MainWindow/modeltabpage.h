#ifndef MODELTABPAGE_H
#define MODELTABPAGE_H

#include <QWidget>
#include <QToolButton>
#include <QHBoxLayout>

class ModelTabPage : public QWidget
{
    Q_OBJECT
public:
    explicit ModelTabPage(QWidget *parent = 0);
    
    void stateMachineWidget();

signals:
    
public slots:
    void OnAddModelBtnSlot();
    void OnViewInMaxBtnSlot();
    void OnEditModelBtnSlot();

    void OnUpdateBtnSlot();

public:
    QToolButton *m_addModelBtn;
    QToolButton *m_viewInMaxBtn;
    QToolButton *m_editModelBtn;

};
#endif // MODELTABPAGE_H
