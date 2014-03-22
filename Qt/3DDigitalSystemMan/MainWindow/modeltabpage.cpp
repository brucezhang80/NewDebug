#include "modeltabpage.h"
#include "mainwindow.h"
#include "addmodelwidget.h"
#include "modelmanager.h"

#include <QGroupBox>
#include <QPropertyAnimation>
#include <QMessageBox>
//状态机
#include <QStateMachine>
#include <QSignalTransition>
#include <QPropertyAnimation>

ModelTabPage::ModelTabPage(QWidget *parent) :
    QWidget(parent)
{
    m_addModelBtn = new QToolButton();
    m_viewInMaxBtn = new QToolButton();
    m_editModelBtn = new QToolButton();
    m_addModelBtn ->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_viewInMaxBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_editModelBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    m_addModelBtn->setFixedSize(60, 72);
    m_viewInMaxBtn->setFixedSize(60, 72);
    m_editModelBtn->setFixedSize(60, 72);

    m_addModelBtn->setText(tr("添加模型"));
    m_viewInMaxBtn->setText(tr("打开模型"));
    m_editModelBtn->setText(tr("编辑模型"));

    m_addModelBtn->setToolTip(tr("添加模型"));
    m_viewInMaxBtn->setToolTip(tr("打开模型"));
    m_editModelBtn->setToolTip(tr("编辑模型"));

    QIcon openmodelIcon(":/icon/open_model.png");
    QIcon viewinmaxIcon(":/icon/view_in_max.png");
    QIcon editmodelIcon(":/icon/edit_model.png");
    m_addModelBtn->setIcon( openmodelIcon );
    m_viewInMaxBtn->setIcon( viewinmaxIcon );
    m_editModelBtn->setIcon( editmodelIcon );
    m_addModelBtn->setIconSize(QSize(48,48));
    m_viewInMaxBtn->setIconSize(QSize(48,48));
    m_editModelBtn->setIconSize(QSize(48,48));

    m_addModelBtn->setCheckable(1);
    m_viewInMaxBtn->setCheckable(1);
    m_editModelBtn->setCheckable(1);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget( m_addModelBtn );
    layout->addWidget( m_viewInMaxBtn );
    layout->addWidget( m_editModelBtn );

    QGroupBox *groupBox = new QGroupBox(this);
    groupBox->setLayout(layout);

    connect(m_addModelBtn, SIGNAL(clicked()), this, SLOT(OnAddModelBtnSlot()));
    connect(m_viewInMaxBtn, SIGNAL(clicked()), this, SLOT(OnViewInMaxBtnSlot()));
    connect(m_editModelBtn, SIGNAL(clicked()), this, SLOT(OnEditModelBtnSlot()));

}

void ModelTabPage::OnAddModelBtnSlot()
{
    MainWindow* main = static_cast<MainWindow*>( this->window() );
    if(!main)
    {
        QMessageBox::warning(this->window(), tr("未知错误:无法解析的主应用程序窗口指针"), tr("访问出错"), QMessageBox::Ok);
        return;
    }

    m_viewInMaxBtn->setChecked(0);
    m_editModelBtn->setChecked(0);

    connect( main->m_addModelWidget, SIGNAL(hideAddModelWgtSignal()), this, SLOT(OnUpdateBtnSlot()) );
}

void ModelTabPage::OnViewInMaxBtnSlot()
{
    m_editModelBtn->setChecked(0);
    m_addModelBtn->setChecked(0);
}

void ModelTabPage::OnEditModelBtnSlot()
{
    m_addModelBtn->setChecked(0);
    m_viewInMaxBtn->setChecked(0);
}

void ModelTabPage::OnUpdateBtnSlot()
{
    m_addModelBtn->setChecked(0);
    m_viewInMaxBtn->setChecked(0);
    m_editModelBtn->setChecked(0);

    MainWindow* main = static_cast<MainWindow*>( this->window() );
    disconnect( main->m_addModelWidget, SIGNAL(hideAddModelWgtSignal()), this, SLOT(OnUpdateBtnSlot()) );
}

void ModelTabPage::stateMachineWidget()
{
    MainWindow* main = static_cast<MainWindow*>( this->window() );
    if(!main)
    {
        return;
    }
    QRect rect1 = main->m_addModelWidget->geometry();
    QRect rect = QRect(2,-rect1.height(), rect1.width(),rect1.height());
    QStateMachine *machine = new QStateMachine;     //新建状态机
    QState *state1 = new QState(machine);     //状态1
    state1->assignProperty(main->m_addModelWidget,"geometry", QRect(rect.x(), rect.y(), rect.width(), rect.height()));    //绑定geomertry属性
    machine->setInitialState(state1);     //state1设为初始化状态
    QState *state2 = new QState(machine);
    state2->assignProperty(main->m_addModelWidget, "geometry", QRect(rect.x(), 2, rect.width(), rect.height()));

    QPropertyAnimation *ani=new QPropertyAnimation(main->m_addModelWidget,"geometry");
    ani->setDuration(1000);
    ani->setEasingCurve(QEasingCurve::OutBounce);   //动画效果—弹跳

    //QSignalTransition *transition1 = state2->addTransition(main->m_addModelWidget->m_pbtnOk, SIGNAL(clicked()),state1);      //动画触发信号
    //transition1->addAnimation(ani);
    QSignalTransition *transition2 = state2->addTransition(main->m_addModelWidget->m_pbtnCancel, SIGNAL(clicked()),state1);
    transition2->addAnimation(ani);
    //QSignalTransition *transition1 = state2->addTransition(main->m_modelManager->m_addAction, SIGNAL(clicked()),state1);
    //transition1->addAnimation(ani);

    QSignalTransition *transition3 = state2->addTransition(m_addModelBtn, SIGNAL(clicked()),state1);
    transition3->addAnimation(ani);
    QSignalTransition *transition5 = state2->addTransition(m_viewInMaxBtn, SIGNAL(clicked()),state1);
    transition5->addAnimation(ani);
    QSignalTransition *transition6 = state2->addTransition(m_editModelBtn, SIGNAL(clicked()),state1);
    transition6->addAnimation(ani);

    QSignalTransition *transition4 = state1->addTransition(m_addModelBtn, SIGNAL(clicked()),state2);
    transition4->addAnimation(ani);

    machine->start();     //开启状态机
}
